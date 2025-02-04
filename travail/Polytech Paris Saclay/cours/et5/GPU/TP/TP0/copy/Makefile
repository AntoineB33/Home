#####################################################################################
## CUDA + OpenMP Matrix product lab
## Makefile by S. Vialle, january 2019
#####################################################################################

CPUCC = g++
GPUCC = /usr/local/cuda/bin/nvcc

CUDA_TARGET_FLAGS = -arch=sm_61 
CXXFLAGS = -O3 
CXXFLAGS += -I/usr/local/cuda/include/
CC_CXXFLAGS = -fopenmp -Wall
CUDA_CXXFLAGS = $(CUDA_TARGET_FLAGS)

CC_LDFLAGS =  -fopenmp 
CUDA_LDFLAGS = -L/usr/local/cuda/lib64/ 

CC_LIBS = 
CUDA_LIBS = -lcudart -lcuda

CC_SOURCES =  main.cpp lodepng.cpp cpu_kernel.cpp
CUDA_SOURCES = gpu_kernel.cu
CC_OBJECTS = $(CC_SOURCES:%.cpp=%.o)
CUDA_OBJECTS = $(CUDA_SOURCES:%.cu=%.o)

EXECNAME = filter


all:
	$(CPUCC) -c $(CC_SOURCES) $(CXXFLAGS) $(CC_CXXFLAGS)
	$(GPUCC) -c $(CUDA_SOURCES) $(CXXFLAGS) $(CUDA_CXXFLAGS)
	$(CPUCC) -o $(EXECNAME) $(CC_LDFLAGS) $(CUDA_LDFLAGS) $(CC_OBJECTS) $(CUDA_OBJECTS) $(CUDA_LIBS) $(CC_LIBS)


clean:
	rm -f *.o $(EXECNAME) *.linkinfo *~ *.bak .depend


#Regles automatiques pour les objets
#%.o:  %.cc
#	$(CPUCC)  -c  $(CXXFLAGS) $(CC_CXXFLAGS) $<
#
#%.o:  %.cu
#	$(GPUCC)  -c  $(CXXFLAGS) $(CUDA_CXXFLAGS) $<

