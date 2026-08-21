#include <string>
#include <vector>
#include "core/color/color_types.hpp"
#include "core/encoder/pixel_to_text.cpp"

namespace Dht {

std::string frameToText(const std::vector<RGB>& pixels, int width, int height) {
    return pixelToText(pixels, width, height);
}

}
