#include <jpeglib.h>
#include <vector>
#include <cstdio>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

namespace Dht {

void writeJPEG(const std::string& path, const std::vector<RGB>& pixels, int width, int height, int quality = 90) {
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        throw DhtError("Cannot create JPEG file: " + path);
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    std::vector<unsigned char> row_buffer(width * 3);
    while (cinfo.next_scanline < cinfo.image_height) {
        int y = cinfo.next_scanline;
        for (int x = 0; x < width; x++) {
            RGB color = pixels[y * width + x];
            row_buffer[x * 3] = color.r;
            row_buffer[x * 3 + 1] = color.g;
            row_buffer[x * 3 + 2] = color.b;
        }
        jpeg_write_scanlines(&cinfo, &row_buffer.data(), 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);
}

}
