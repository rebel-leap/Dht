#include <cmath>
#include <string>
#include "color_types.hpp"

namespace Dht {

std::string rgbToCode(const RGB& color) {
    std::string code;

    for (const auto& pair : BASE_COLORS) {
        char letter = pair.first;
        RGB base = pair.second;

        double dist = std::sqrt(
            std::pow(color.r - base.r, 2) +
            std::pow(color.g - base.g, 2) +
            std::pow(color.b - base.b, 2)
        );

        if (dist < 100.0) {
            uint8_t channel;
            if (letter == 'R') channel = color.r;
            else if (letter == 'G') channel = color.g;
            else channel = color.b;

            code += letter;
            if (channel > 200) code += 'L';
            else if (channel < 55) code += 'D';
        }
    }

    if (code.empty()) {
        if (color.r < 30 && color.g < 30 && color.b < 30) return "RGBD";
        return "RGB";
    }

    return code;
}

}
