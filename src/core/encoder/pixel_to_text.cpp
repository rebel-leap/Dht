#include <string>
#include <vector>
#include "core/color/color_types.hpp"
#include "core/color/rgb_to_code.cpp"

namespace Dht {

std::string pixelToText(const std::vector<RGB>& pixels, int width, int height) {
    std::string result;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            result += rgbToCode(pixels[y * width + x]);
            if (x < width - 1) result += ' ';
        }
        if (y < height - 1) result += '\n';
    }

    return result;
}

}
