#include <iostream>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <chrono>

#include "lodepng.h"
#include "gpu_kernel.h"
#include "cpu_kernel.h"

#define FILTER_LIMIT 100

void welcome() {
    std::cout << "** Execution of our CPU/GPU image filter **" << std::endl;
    std::cout << "Parameters: [input] [output] [filter level] [mode]" << std::endl;
    std::cout << "where" << std::endl;
    std::cout << "input: input file name" << std::endl;
    std::cout << "output: output file name" << std::endl;
    std::cout << "filter level: level of filtering (between 1 and " << FILTER_LIMIT
        << ")" << std::endl;
    std::cout << "mode: 'cpu' or 'gpu' execution" << std::endl << std::endl;
}

int main(int argc, char** argv) {

    /** Starting message **/
    welcome();

    /** Argument parsing **/
    if(argc != 5) {
        std::cout << "Missing arguments." << std::endl;
        return 1; // stops execution
    }

    // Read the arguments
    const char* input_file = argv[1];
    const char* output_file = argv[2];

    unsigned int filter_level = atoi(argv[3]);
    if(filter_level == 0 || filter_level > FILTER_LIMIT) {
        std::cout << "Filter level outside the limits." << std::endl;
        return 1; // stops execution
    }

    bool is_gpu_kernel;
    if (std::strcmp(argv[4],"gpu") == 0) {
        is_gpu_kernel = true; // we should run the GPU version
    } else {
        if (std::strcmp(argv[4],"cpu") == 0) {
            is_gpu_kernel = false; // we should run the CPU version
        } else { // we do not know which version to run
            std::cout << "Last argument must be 'cpu' or 'gpu'." << std::endl;
            return 1; // stops execution
        }
    }

    /** CUDA initilization to remove its overhead from the measurements **/
    cuInit(0);
    cudaDeviceSynchronize();

    /** Image reading **/
    std::vector<unsigned char> in_image;
    unsigned int width, height;

    // Load the data
    unsigned error = lodepng::decode(in_image, width, height, input_file);
    if(error) { //something went wrong when reading the input file
        std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1; // stops execution
    }

    // Prepare the data
    // Images are represented in RGB, where each color is the size of one byte
    unsigned char* input_image = new unsigned char[(in_image.size()*3)/4];
    unsigned char* tmp_image = new unsigned char[(in_image.size()*3)/4];
    unsigned char* output_image; // pointer to input_image or tmp_image (based on filter level)
    unsigned int where = 0;
    for(unsigned int i = 0; i < in_image.size(); ++i) {
        if((i+1) % 4 != 0) {
            input_image[where] = in_image.at(i);
            where++;
        }
    }

    /** Image filtering **/
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    if(is_gpu_kernel) {
        // function on gpu_kernel.cu
        output_image = gpuFilter(input_image, tmp_image, width, height, filter_level);
    } else {
        // function on cpu_kernel.cpp
        output_image = cpuFilter(input_image, tmp_image, width, height, filter_level);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Total execution time of filter: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()
        << " ms" << std::endl;

    /** Image writing **/
    std::vector<unsigned char> out_image;
    for(unsigned int i = 0; i < in_image.size(); ++i) {
        out_image.push_back(output_image[i]);
        if((i+1) % 3 == 0) {
            out_image.push_back(255);
        }
    }

    // Output the data
    error = lodepng::encode(output_file, out_image, width, height);
    if(error) std::cout << "Encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    delete[] input_image;
    delete[] tmp_image;
    return 0;

}
