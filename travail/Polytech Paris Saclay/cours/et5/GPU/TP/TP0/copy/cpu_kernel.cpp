#include "cpu_kernel.h"

// ---------------------------------------------------------
// CPU blur kernel 
// ---------------------------------------------------------
void cpuBlur(
        unsigned char* input_image,
        unsigned char* output_image,
        unsigned int width,
        unsigned int height) {

    // The offset defines the pixel that will be computed
#pragma omp parallel for
    for(unsigned int offset = 0; offset < width*height; ++offset) {
        // x and y represent the positionl of the pixel in the image
        int x = offset % width;
        int y = (offset-x)/width;
        int fsize = FSIZE; // Size of the filter to use for bluring
        // Temporary variables to accumulate the values of RGB in the neighborhood
        unsigned int output_red = 0;
        unsigned int output_green = 0;
        unsigned int output_blue = 0;
        unsigned int hits = 0; // Number of valid pixels (insize the image) used

        // Iterates over the pixels in the neighborhood
        for(int ox = -fsize; ox < fsize+1; ++ox) {
            for(int oy = -fsize; oy < fsize+1; ++oy) {
                // Check if the pixel is valid
                if((x+ox) >= 0
                        && ((unsigned int)(x+ox)) < width
                        && (y+oy) >= 0
                        && ((unsigned int)(y+oy)) < height) {

                    const int currentoffset = (offset+ox+oy*width)*3;
                    output_red += input_image[currentoffset];
                    output_green += input_image[currentoffset+1];
                    output_blue += input_image[currentoffset+2];
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


// ---------------------------------------------------------
// Launching a filtering on CPU 
// ---------------------------------------------------------
unsigned char* cpuFilter (
        unsigned char* input_image,
        unsigned char* tmp_image,
        unsigned int width,
        unsigned int height,
        unsigned int filter_level) {

    std::cout << "Running the filter on CPU " << filter_level
        << " times for an image of dimensions " << width << " x " << height << std::endl;

    // References to the original data structures
    unsigned char* in = input_image;
    unsigned char* out = tmp_image;

    for(unsigned int i = 0; i < filter_level; ++i){
        // Function call
        cpuBlur(in, out, width, height);

        // Swaps references except on the last iteration
        if (i < filter_level-1){
            // reference swap
            unsigned char* swap_tmp = out;
            out = in;
            in = swap_tmp;
        }
    }

    return out;
}

