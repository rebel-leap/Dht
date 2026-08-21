#include <jpeglib.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

std::vector<RGB> readJPEG(const std::string& path, int& width, int& height) {
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundError(path);
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;
    int channels = cinfo.output_components;

    std::vector<unsigned char> row_buffer(width * channels);
    std::vector<unsigned char> image_data(width * height * channels);

    int row = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &row_buffer.data(), 1);
        std::memcpy(image_data.data() + row * width * channels, row_buffer.data(), width * channels);
        row++;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    std::vector<RGB> pixels(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            size_t idx = (y * width + x) * channels;
            if (channels >= 3) {
                pixels[y * width + x] = RGB(image_data[idx], image_data[idx + 1], image_data[idx + 2]);
            } else {
                pixels[y * width + x] = RGB(image_data[idx], image_data[idx], image_data[idx]);
            }
        }
    }

    return pixels;
}

}
