#include <vector>
#include <cstdio>
#include <cstring>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

void writeBMP(const std::string& path, const std::vector<RGB>& pixels, int width, int height) {
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        throw DhtError("Cannot create BMP file: " + path);
    }

    int rowSize = ((24 * width + 31) / 32) * 4;
    int imageSize = rowSize * height;
    int fileSize = 54 + imageSize;

    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(int*)&header[2] = fileSize;
    *(int*)&header[10] = 54;
    *(int*)&header[14] = 40;
    *(int*)&header[18] = width;
    *(int*)&header[22] = height;
    *(short*)&header[26] = 1;
    *(short*)&header[28] = 24;
    *(int*)&header[34] = imageSize;

    fwrite(header, 1, 54, fp);

    std::vector<unsigned char> row_data(rowSize, 0);
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            RGB color = pixels[y * width + x];
            row_data[x * 3] = color.b;
            row_data[x * 3 + 1] = color.g;
            row_data[x * 3 + 2] = color.r;
        }
        fwrite(row_data.data(), 1, rowSize, fp);
    }

    fclose(fp);
}

}
