#include <string>
#include <vector>
#include <sstream>
#include "core/color/color_types.hpp"
#include "core/color/code_to_rgb.cpp"

namespace Dht {

std::vector<RGB> textToPixel(const std::string& text, int& width, int& height) {
    std::vector<RGB> pixels;
    std::istringstream iss(text);
    std::string line;
    height = 0;
    width = 0;

    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        std::istringstream lineStream(line);
        std::string code;
        while (lineStream >> code) {
            pixels.push_back(codeToRGB(code));
        }
        if (width == 0) width = pixels.size() / (height + 1);
        ++height;
    }

    return pixels;
}

}
