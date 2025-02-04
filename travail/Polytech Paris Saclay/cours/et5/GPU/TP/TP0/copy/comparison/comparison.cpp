#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>

#include "lodepng.h"

void welcome() {
    std::cout << "** Comparison of two PNG files **" << std::endl;
    std::cout << "Parameters: [image1] [image2]" << std::endl;
    std::cout << "where" << std::endl;
    std::cout << "image1: first PNG image" << std::endl;
    std::cout << "image2: second PNG image" << std::endl << std::endl;
}

int main(int argc, char** argv) {

    /** Starting message **/
    welcome();

    /** Argument parsing **/
    if(argc != 3) {
        std::cout << "Missing arguments." << std::endl;
        return 1; // stops execution
    }

    // Read the arguments
    const char* first_file = argv[1];
    const char* second_file = argv[2];

    /** Image reading - 1 **/
    std::vector<unsigned char> first_image;
    unsigned int first_width, first_height;
    unsigned error = lodepng::decode(first_image, first_width, first_height, first_file);
    if(error) { //something went wrong when reading the input file
        std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1; // stops execution
    }

    /** Image reading - 2 **/
    std::vector<unsigned char> second_image;
    unsigned int second_width, second_height;
    error = lodepng::decode(second_image, second_width, second_height, second_file);
    if(error) { //something went wrong when reading the input file
        std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1; // stops execution
    }

    /** First comparison - sizes **/
    if(first_width != second_width || first_height != second_height){
        std::cout << "The images have different dimensions. Stopping comparison!" <<std::endl;
        return 1;
    }

    std::cout << "Comparing images of dimensions " << first_width << " X " << first_height << std::endl;
    /** Second comparison - for each color in each pixel **/
    unsigned int total_difference = 0;
    unsigned char maximum_difference = 0;
    for(unsigned int i = 0; i < first_image.size(); ++i) {
        if((i+1) % 4 != 0) {
            unsigned char first_color = first_image.at(i);
            unsigned char second_color = second_image.at(i);

            if(first_color != second_color){

                unsigned char difference =
                    first_color > second_color ?
                    first_color - second_color :
                    second_color - first_color;

                total_difference += difference;
                if(difference > maximum_difference)
                    maximum_difference = difference;
            }
        }
    }
    // Total number of colors checked = height * width * 3 colors per pixel
    double average_difference = total_difference / (first_height*first_width*3);

    /** Output **/
    std::cout << "Maximum color difference (up to 255): " << (int) maximum_difference << std::endl;
    std::cout << "Total color difference: " << total_difference << std::endl;
    std::cout << "Average color difference: " << average_difference << std::endl << std::endl;

    return 0;

}
