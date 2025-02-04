#pragma once

// Size of the filter for bluring
// e.g., if FSIZE = 5, then pixels that are at most 5 to the left,
// 5 to the right, 5 up, or 5 down from the original pixel will be considered,
// making it a 11x11 filter
#define FSIZE 5

#define BLOCK_SIZE_1 512 // size of a block of threads in the GPU
#define BLOCK_SIZE_2 1024 // size of a block of threads in the GPU
