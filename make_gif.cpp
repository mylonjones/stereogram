#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GIF_H_IMPLEMENTATION
#include "gif.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    int width, height, channels;

    
    GifWriter writer = {};
    int delay = 10; // hundredths of a second (10 = 0.1s)
    
    
    for (int i = 1; i <= 10; i++) {
        stringstream filename;
        filename << "./frames/frame" << i << ".jpg";
        unsigned char* img = stbi_load(filename.str().c_str(), &width, &height, &channels, 4);
        if (!img) {
            cerr << "Failed to load " << filename.str().c_str() << endl;
            return 1;
        }

        if (i == 1) GifBegin(&writer, "stereogram.gif", width, height, delay);
        GifWriteFrame(&writer, img, width, height, delay);
        cout << "added frame " << i << endl;
        stbi_image_free(img);
    }

    GifEnd(&writer);
    return 0;
}
