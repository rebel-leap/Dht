#include <png.h>
#include <vector>
#include <cstdio>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

std::vector<RGB> readPNG(const std::string& path, int& width, int& height) {
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundError(path);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        throw DhtError("Failed to create PNG read struct");
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(fp);
        throw DhtError("Failed to create PNG info struct");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
        throw DhtError("PNG read error");
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) png_set_strip_alpha(png);

    png_read_update_info(png, info);

    std::vector<RGB> pixels(width * height);
    std::vector<png_bytep> row_pointers(height);
    std::vector<png_byte> image_data(height * width * 3);

    for (int y = 0; y < height; y++) {
        row_pointers[y] = image_data.data() + y * width * 3;
    }

    png_read_image(png, row_pointers.data());
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            size_t idx = (y * width + x) * 3;
            pixels[y * width + x] = RGB(image_data[idx], image_data[idx + 1], image_data[idx + 2]);
        }
    }

    return pixels;
}

}
