#pragma once

// Dependencies --------------------------------------------
#include <iostream>
#include <cmath>
#include <omp.h>
#include "constants.h"


// Dependencies --------------------------------------------
unsigned char* cpuFilter(
        unsigned char* input_image,
        unsigned char* tmp_image,
        unsigned int width,
        unsigned int height,
        unsigned int filter_level);


// CPU Kernels ---------------------------------------------
void cpuBlur(
        unsigned char* input_image,
        unsigned char* output_image,
        unsigned int width,
        unsigned int height);
