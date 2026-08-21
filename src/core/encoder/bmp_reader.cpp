#include <vector>
#include <cstdio>
#include <cstring>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

std::vector<RGB> readBMP(const std::string& path, int& width, int& height) {
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundError(path);
    }

    unsigned char header[54];
    fread(header, 1, 54, fp);

    width = *(int*)&header[18];
    height = *(int*)&header[22];
    int bitsPerPixel = *(short*)&header[28];
    int rowSize = ((bitsPerPixel * width + 31) / 32) * 4;
    int dataOffset = *(int*)&header[10];

    fseek(fp, dataOffset, SEEK_SET);
    std::vector<unsigned char> image_data(height * rowSize);
    fread(image_data.data(), 1, image_data.size(), fp);
    fclose(fp);

    std::vector<RGB> pixels(width * height);
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            size_t idx = y * rowSize + x * (bitsPerPixel / 8);
            if (bitsPerPixel >= 24) {
                pixels[(height - 1 - y) * width + x] = RGB(image_data[idx + 2], image_data[idx + 1], image_data[idx]);
            } else {
                pixels[(height - 1 - y) * width + x] = RGB(image_data[idx], image_data[idx], image_data[idx]);
            }
        }
    }

    return pixels;
}

}
