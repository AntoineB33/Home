#include "gpu_kernel.h"


// ----------------------------------------------------------------------------
// GPU blur kernel
// ----------------------------------------------------------------------------
__global__ void gpuBlur(
        unsigned char* input_image, 
        unsigned char* output_image, 
        unsigned int width, 
        unsigned int height) {

    // The offset defines the pixel that will be computed by one thread
    // const unsigned int offset = blockIdx.y*blockDim.x + threadIdx.x;
    const unsigned int offset = blockIdx.x*blockDim.x + threadIdx.x;

    // x and y represent the position of the pixel in the image
    int x = offset % width;
    int y = (offset-x)/width;
    int fsize = FSIZE; // Size of the filter to use for bluring

    // Making sure we are only computing pixels inside the image
    if(offset < width*height) {
        // Temporary variables to accumulate the values of RGB in the neighborhood
        unsigned int output_red = 0;
        unsigned int output_green = 0;
        unsigned int output_blue = 0;
        unsigned int hits = 0; // Number of valid pixels (insize the image) used

        // Iterates over the pixels in the neighborhood
        for(int ox = -fsize; ox < fsize+1; ++ox) {
            for(int oy = -fsize; oy < fsize+1; ++oy) {
                if(x + ox >= 0 && x + ox < width && y + oy >= 0 && y + oy < height) {
                    const int currentoffset = (offset+ox+oy*width)*3;
                    output_red += 255 - input_image[currentoffset]; 
                    output_green += 255 - input_image[currentoffset+1];
                    output_blue += 255 - input_image[currentoffset+2];
                    hits++;
                }
            }
        }


        // The output value is the average from the neighborhood
        output_image[offset*3] = output_red/hits;
        output_image[offset*3+1] = output_green/hits;
        output_image[offset*3+2] = output_blue/hits;
    }
}


// ----------------------------------------------------------------------------
// Launching a filtering on GPU
// ----------------------------------------------------------------------------
unsigned char* gpuFilter(
        unsigned char* input_image, 
        unsigned char* output_image, 
        unsigned int width, 
        unsigned int height, 
        unsigned int filter_level) {

    std::cout << "Running the filter on GPU " << filter_level 
        << " times for an image of dimensions " << width << " x " << height << std::endl;

    // References to the data structures in the GPU
    unsigned char* dev_input;
    unsigned char* dev_output;

    // Memory allocation and copy to the GPU
    // For each pixel in width*height, we have 3 colors (RGB)
    getError(cudaMalloc((void**) &dev_input, width*height*3*sizeof(unsigned char)),__LINE__);
    getError(cudaMemcpy(dev_input, input_image, width*height*3*sizeof(unsigned char),
                cudaMemcpyHostToDevice),__LINE__);
    // Memory allocation of the output image in the GPU (no need to copy data)
    getError(cudaMalloc((void**) &dev_output, width*height*3*sizeof(unsigned char)),__LINE__);


    dim3 blockDims(BLOCK_SIZE_1,1,1);
    dim3 gridDims((unsigned int) ceil(width*height*3/((double)BLOCK_SIZE_2)), 1, 1 );

    for(unsigned int i = 0; i < filter_level; ++i) {
        //Function call
        gpuBlur<<<gridDims, blockDims>>>(dev_input, dev_output, width, height); 

        //Swaps references except on the last iteration
        if (i < filter_level-1){
            // reference swap
            unsigned char* swap_tmp = dev_output;
            dev_output = dev_input;
            dev_input = swap_tmp;
        }
    }

    // Copies data back from the GPU
    getError(cudaMemcpy(output_image, dev_output, width*height*3*sizeof(unsigned char),
                cudaMemcpyDeviceToHost),__LINE__);
    // Frees allocated memory in the GPU
    getError(cudaFree(dev_input),__LINE__);
    getError(cudaFree(dev_output),__LINE__);

    return output_image;
}


// ----------------------------------------------------------------------------
// Error reporting 
// ----------------------------------------------------------------------------
void getError(cudaError_t err, int line) {
    if(err != cudaSuccess) {
        std::cout << "!! Error on a CUDA function call on line " 
            << line << " : " << cudaGetErrorString(err) << std::endl;
    }
}

