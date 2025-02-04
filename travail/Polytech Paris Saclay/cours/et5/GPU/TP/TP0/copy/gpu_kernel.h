#pragma once

// Dependencies --------------------------------------------
#include <cmath>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include "constants.h"

// Functions run in the CPU to drive the GPU ---------------
unsigned char* gpuFilter(
        unsigned char* input_image,
        unsigned char* tmp_image,
        unsigned int width,
        unsigned int height,
        unsigned int filter_level);

void getError(cudaError_t err, int line);
