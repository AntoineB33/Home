/*********************************************************************************/
/* Matrix product program for a multi-core CPU and for a many-core GPU           */
/* S. Vialle - December 2024                                                     */
/*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h> 
#include <cuda_runtime.h>
#include <cublas_v2.h>

#include "main.h"
#include "gpu.h"


/*-------------------------------------------------------------------------------*/
/* GPU symbols and global vars                                                   */
/*-------------------------------------------------------------------------------*/
// Symbols used by all kernels
__device__ T_real GPU_A[SIZE][SIZE];
__device__ T_real GPU_B[SIZE][SIZE];
__device__ T_real GPU_C[SIZE][SIZE];

// New Symbol and vars to call Cublas lib.
__device__ T_real GPU_Ctmp[SIZE][SIZE];   // New matrix buffer

T_real *AdrGPU_A = NULL;                  // Adresses of the symbols
T_real *AdrGPU_B = NULL;
T_real *AdrGPU_C = NULL;
T_real *AdrGPU_Ctmp = NULL; 

cublasHandle_t cublasHandle;              // Handle on the Cublas lib.


/*-------------------------------------------------------------------------------*/
/* Init and finalize the GPU device.                                             */
/*-------------------------------------------------------------------------------*/
void gpuInit(void)
{
  cuInit(0);
  
  // Extract address of GPU matrix "symbols"
  CHECK_CUDA_SUCCESS(cudaGetSymbolAddress((void **)&AdrGPU_A,GPU_A),"GPU_A adr extraction");
  CHECK_CUDA_SUCCESS(cudaGetSymbolAddress((void **)&AdrGPU_B,GPU_B),"GPU_B adr extraction");
  CHECK_CUDA_SUCCESS(cudaGetSymbolAddress((void **)&AdrGPU_C,GPU_C),"GPU_C adr extraction");
  CHECK_CUDA_SUCCESS(cudaGetSymbolAddress((void **)&AdrGPU_Ctmp,GPU_Ctmp),"GPU_Ctmp adr extraction");
  
  // Turn CPU arrays A, B and C into "pinned" memory areas
  /**/
  CHECK_CUDA_SUCCESS(cudaHostRegister(A,SIZE*SIZE*sizeof(T_real),
                                      cudaHostRegisterPortable),
                     "Turning into pinned memory the A CPU array");
  CHECK_CUDA_SUCCESS(cudaHostRegister(B,SIZE*SIZE*sizeof(T_real),
                                      cudaHostRegisterPortable),
                     "Turning into pinned memory the B CPU array");
  CHECK_CUDA_SUCCESS(cudaHostRegister(C,SIZE*SIZE*sizeof(T_real),
                                      cudaHostRegisterPortable),
                     "Turning into pinned memory the C CPU array");
  /**/
  
  // Initialize CUBLAS lib usage
  CHECK_CUBLAS_SUCCESS(cublasCreate(&cublasHandle), "Init of the CUBLAS lib handle"); 
}


void gpuFinalize(void)
{
  // Turn "pinned" CPU arrays into std array
  /**/
  CHECK_CUDA_SUCCESS(cudaHostUnregister(A),
                     "Turning into std memory the A CPU array");
  CHECK_CUDA_SUCCESS(cudaHostUnregister(B),
                     "Turning into std memory the B CPU array");
  CHECK_CUDA_SUCCESS(cudaHostUnregister(C),
                     "Turning into std memory the C CPU array");
  /**/

  // Free CUBLAS lib usage
  CHECK_CUBLAS_SUCCESS(cublasDestroy(cublasHandle), "Free the CUBLAS lib");
}


/*-------------------------------------------------------------------------------*/
/* Transfer of CPU input data into GPU symbols                                   */
/*-------------------------------------------------------------------------------*/
void gpuSetDataOnGPU(void)
{
 // Set GPU_A symbol
 CHECK_CUDA_SUCCESS(cudaMemcpyToSymbol(GPU_A,A,SIZE*SIZE*sizeof(T_real),0,cudaMemcpyHostToDevice),
                    "Transfer A-->GPU_A");

 // Set GPU_B symbol
 CHECK_CUDA_SUCCESS(cudaMemcpyToSymbol(GPU_B,B,SIZE*SIZE*sizeof(T_real),0,cudaMemcpyHostToDevice),
                    "Transfer B-->GPU_B");
}


/*-------------------------------------------------------------------------------*/
/* Transfer of GPU results into CPU array                                        */
/*-------------------------------------------------------------------------------*/
void gpuGetResultOnCPU(void)
{
 // Get GPU_C symbol
 CHECK_CUDA_SUCCESS(cudaMemcpyFromSymbol(C,GPU_C,SIZE*SIZE*sizeof(T_real),0,cudaMemcpyDeviceToHost),
                    "Transfer GPU_C-->C");
}


/*-------------------------------------------------------------------------------*/
/* Transposition kernel using global memory and registers.                       */
/*-------------------------------------------------------------------------------*/
__global__ void TransposeKernel_v0(T_real *MT, T_real *M, int mLig, int nCol)
{
 int lig = threadIdx.y + blockIdx.y*BLOCK_SIZE_XY_KT0;
 int col = threadIdx.x + blockIdx.x*BLOCK_SIZE_XY_KT0;
 
 if (lig < mLig && col < nCol)
   MT[col*mLig + lig] = M[lig*nCol + col];
}


/*-------------------------------------------------------------------------------*/
/* Transposition kernel using global shared memory.                              */
/*-------------------------------------------------------------------------------*/
__global__ void TransposeKernel_v1(T_real *MT, T_real *M, int mLig, int nCol)
{
 // Natural lig-col coordinates of the thread (leading to coalescent reading)
 int firstLibBlock = blockIdx.y*BLOCK_SIZE_XY_KT1;
 int firstColBlock = blockIdx.x*BLOCK_SIZE_XY_KT1;
 int lig = firstLibBlock + threadIdx.y;
 int col = firstColBlock + threadIdx.x;
 
 // Not natural ligT-colT coordinates of the threadin the transpose matrix
 // in order to achieve coalescent writting
 int ligT = firstColBlock + threadIdx.y;
 int colT = firstLibBlock + threadIdx.x;
 
 // shared memory matrix block
 __shared__ T_real shM[BLOCK_SIZE_XY_KT1][BLOCK_SIZE_XY_KT1];
 
 // Coalescent reading of M matrix, and storage in shm matrix block
 if (lig < mLig && col < nCol)
   shM[threadIdx.y][threadIdx.x] = M[lig*nCol + col];
 // Waiting all data are stored in the shm block matrix
 __syncthreads();
 // Coalescent writing in the transposed matrix
 if (ligT < nCol && colT < mLig)
   MT[(firstColBlock + threadIdx.y)*mLig + 
      (firstLibBlock + threadIdx.x)        ] = shM[threadIdx.x][threadIdx.y];
}


/*-------------------------------------------------------------------------------*/
/* Small matrix product on the local GPU - 1D & generic matrix size              */
/*-------------------------------------------------------------------------------*/
__global__ void MatrixProductKernel_v0(void)
{
 // Index computations
 int lig = blockIdx.y;
 int col = threadIdx.x + blockIdx.x*BLOCK_SIZE_X_K0;

 // Matrix product computation
 if (col < SIZE /* && lig < SIZE */) {
   T_real accu = 0.0;
   for (int k = 0; k < SIZE; k++) {
     accu += GPU_A[lig][k]*GPU_B[k][col];
   }
   GPU_C[lig][col] = accu;
 }
}


/*-------------------------------------------------------------------------------*/
/* Small matrix product on the local GPU - 2D & generic matrix size              */
/*-------------------------------------------------------------------------------*/
__global__ void MatrixProductKernel_v1(void)
{
 // Index computations
 int lig = threadIdx.y + blockIdx.y*BLOCK_SIZE_Y_K1;
 int col = threadIdx.x + blockIdx.x*BLOCK_SIZE_X_K1;

 // Matrix product computation
 if (col < SIZE  && lig < SIZE) {
   T_real accu = 0.0;
   for (int k = 0; k < SIZE; k++) {
     accu += GPU_A[lig][k]*GPU_B[k][col];
   }
   GPU_C[lig][col] = accu;
 }
}


/*-------------------------------------------------------------------------------*/
/* Small matrix product on the local GPU.                                        */
/*-------------------------------------------------------------------------------*/
void gpuProduct(gkid_t kid)
{
 dim3 Dg = {0,0,0};   // Grid descriptor
 dim3 Db = {0,0,0};   // Block descriptor
 
 //T_real alpha;        // When using CUBLAS
 //T_real beta;         // When using CUBLAS

 switch(kid) {

  case GK0 : // Kernel v0 - 1D kernel using only resgisters and cache with generic matrix size
   // - init the grid of blocs
   Db.x = BLOCK_SIZE_X_K0;
   Db.y = 1;
   Db.z = 1;
   Dg.x = SIZE/BLOCK_SIZE_X_K0 + (SIZE%BLOCK_SIZE_X_K0 > 0 ? 1 : 0);
   Dg.y = SIZE;
   Dg.z = 1;
   // - run the Grid of Blocs of threads
   MatrixProductKernel_v0<<<Dg,Db>>>();
   break;

 case GK1 : // kernel v1 : 2D kernel using only registers and cache with generic matrix size
   // - init the grid of blocs
   Db.x = BLOCK_SIZE_X_K1;
   Db.y = BLOCK_SIZE_Y_K1;
   Db.z = 1;
   Dg.x = SIZE/BLOCK_SIZE_X_K1 + (SIZE%BLOCK_SIZE_X_K1 > 0 ? 1 : 0);
   Dg.y = SIZE/BLOCK_SIZE_Y_K1 + (SIZE%BLOCK_SIZE_Y_K1 > 0 ? 1 : 0);;
   Dg.z = 1;
   // - run the Grid of Blocs of threads
   MatrixProductKernel_v1<<<Dg,Db>>>();
   break;

 case GK2 : // kernel v2 : 2D kernel using the shared memories
   break;
  
 case GK3 : // kernel v3 : 2D kernel using the shared memories with generic matrix size
   break;

 case GK4 : // calling cublas gemm & user-defined transpose kernel
   break;
   
 case GK5 : // Calling cublas gemm & cublas geam kernels
   break;

 case GK6 : // Calling cublas gemm, using matrix math properties
   break;

 case GK7 : // Calling cublas gemmEx with std 32F/64F datatypes, using Tensor cores
   break;

 case GK8 : // Calling cublas gemmEx with different low precisions on 32/16 bits datatypes, 
            // using Tensor cores
   break;

 default :
   fprintf(stderr,"Unknown GPU kernel!");
   exit(EXIT_FAILURE);
 } // End of switch
}




