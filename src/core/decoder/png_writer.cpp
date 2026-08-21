#include <png.h>
#include <vector>
#include <cstdio>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

void writePNG(const std::string& path, const std::vector<RGB>& pixels, int width, int height) {
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        throw DhtError("Cannot create PNG file: " + path);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        throw DhtError("Failed to create PNG write struct");
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        throw DhtError("Failed to create PNG info struct");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        throw DhtError("PNG write error");
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    std::vector<png_bytep> row_pointers(height);
    std::vector<png_byte> image_data(height * width * 3);

    for (int y = 0; y < height; y++) {
        row_pointers[y] = image_data.data() + y * width * 3;
        for (int x = 0; x < width; x++) {
            size_t idx = (y * width + x) * 3;
            RGB color = pixels[y * width + x];
            image_data[idx] = color.r;
            image_data[idx + 1] = color.g;
            image_data[idx + 2] = color.b;
        }
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

}
