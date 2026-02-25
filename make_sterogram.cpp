#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int coordinates_2d_1d(int x, int y, int width) {
    return (y * width + x) * 3;
}

unsigned char* remove_edges(unsigned char *img, int width, int height) {
    unsigned char* result = new unsigned char[(width - 8) * (height - 8) * 3];

    for (int y = 4; y < height - 4; ++y) {
        for (int x = 4; x < width - 4; ++x) {
            int index = coordinates_2d_1d(x, y, width);
            int result_index = coordinates_2d_1d(x - 4, y - 4, width - 8);
            result[result_index] = img[index];
            result[result_index + 1] = img[index + 1];
            result[result_index + 2] = img[index + 2];
        }
    }

    return result;
}

unsigned char* append_frame(unsigned char *img1, unsigned char *img2, int width1, int height1, int width2, int height2) {
    if (height1 != height2) {
        cout << "Images are not the same height" << endl;
        exit(1);
    };
    int combinedWidth = width1 + width2;
    unsigned char *result = new unsigned char[(combinedWidth) * height1 * 3];
    unsigned char *source;
    int index, source_index;
    for (int y = 0; y < height1; ++y) {
        for (int x = 0; x < combinedWidth; ++x) {
            index = coordinates_2d_1d(x, y, combinedWidth);
            if (x < width1) {
                source_index = coordinates_2d_1d(x, y, width1);
                source = img1;
            } else {
                source_index = coordinates_2d_1d(x - width1, y, width2);
                source = img2;
            }
            result[index] = source[source_index];
            result[index + 1] = source[source_index + 1];
            result[index + 2] = source[source_index + 2];
        }
    }

    return result;
}

void shift_shape(unsigned char *pattern, unsigned char *shape, int width1, int width2, int height) {
     int shift = 30;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width2; ++x) {
            int index = coordinates_2d_1d(x, y, width2);

            double shade = 1 - (shape[index] + shape[index + 1] + shape[index + 2]) / 3.0 / 255;
            if (shade == 0) continue;

            int unshifted_index = coordinates_2d_1d( x + width1 - width2, y, width1);
            int shifted_index = coordinates_2d_1d( x + width1 - width2 - (shade * shift), y, width1);


            pattern[shifted_index] = pattern[unshifted_index];
            pattern[shifted_index + 1] = pattern[unshifted_index + 1];
            pattern[shifted_index + 2] = pattern[unshifted_index + 2];
        }
    }
}

unsigned char* partition_shape(unsigned char *shape, int width, int height, int partitionSize, int partitionIndex) {
    unsigned char *result = new unsigned char[partitionSize * height * 3];
    
    int index, shape_index;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < partitionSize; ++x) {
            index = coordinates_2d_1d(x, y, partitionSize);
            
            shape_index = coordinates_2d_1d(x + partitionIndex * partitionSize, y, width);
            
            result[index] = shape[shape_index];
            result[index + 1] = shape[shape_index + 1];
            result[index + 2] = shape[shape_index + 2];
        }
    }

    return result;
}

int main(int argc, char **argv) {
    int width1, height1, channels1;
    int width2, height2, channels2;

    if (argc != 2) exit(1);
    
    unsigned char* pattern = stbi_load("pattern.jpg", &width1, &height1, &channels1, 3);
    unsigned char* shape = stbi_load("shape.jpg", &width2, &height2, &channels2, 3);
    
    // if (!pattern || !shape) {
    //     printf("Failed to load image\n");
    //     return 1;
    // }
    
    // if (width1 != width2) {
    //     cout << "width does not match" << endl;
    // }
    // if (height1 != height2) {
    //     cout << "height does not match" << endl;
    // }

    
    // cout << "pattern height and width: " << height1 << " " << width1 << endl;
    // cout << "shape height and width: " << height2 << " " << width2 << endl;
    
    unsigned char *result;

    int trimmedPatternWidth = width1 - 8;
    int trimmedPatternHeight = height1 - 8;
    int trimmedShapeWidth = width2 - 8;
    int trimmedShapeHeight = height2 - 8;

    unsigned char *trimmedPattern = remove_edges(pattern, width1, height1);
    unsigned char *trimmedShape = remove_edges(shape, width2, height2);
    
    unsigned char *partition1 = partition_shape(trimmedShape, trimmedShapeWidth, trimmedShapeHeight, trimmedPatternWidth, 0);
    unsigned char *partition2 = partition_shape(trimmedShape, trimmedShapeWidth, trimmedShapeHeight, trimmedPatternWidth, 1);

    result = append_frame(trimmedPattern, trimmedPattern, trimmedPatternWidth, trimmedPatternHeight, trimmedPatternWidth, trimmedPatternHeight);

    shift_shape(result, partition1, trimmedPatternWidth * 2, trimmedPatternWidth, trimmedPatternHeight);

    trimmedPattern = partition_shape(result, trimmedPatternWidth * 2, trimmedPatternHeight, trimmedPatternWidth, 1);

    result = append_frame(result, trimmedPattern, trimmedPatternWidth * 2, trimmedPatternHeight, trimmedPatternWidth, trimmedPatternHeight);
    
    shift_shape(result, partition2, trimmedPatternWidth * 3, trimmedPatternWidth, trimmedPatternHeight);

    stringstream file_name;
    file_name << "./frames/frame" << argv[1] << ".jpg";
    stbi_write_jpg(file_name.str().c_str(), trimmedPatternWidth * 3, trimmedPatternHeight, 3, result, 100);
    cout << "created frame" << argv[1] << ".jpg" << endl;

    return 0;
}
