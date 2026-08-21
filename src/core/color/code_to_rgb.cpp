#include <string>
#include <algorithm>
#include "color_types.hpp"

namespace Dht {

RGB codeToRGB(const std::string& code) {
    RGB result(0, 0, 0);
    int count = 0;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];
        auto it = BASE_COLORS.find(c);
        if (it == BASE_COLORS.end()) continue;

        RGB base = it->second;
        double factor = 1.0;

        if (i + 1 < code.length()) {
            if (code[i + 1] == 'L') { factor = 1.5; ++i; }
            else if (code[i + 1] == 'D') { factor = 0.5; ++i; }
        }

        result.r = std::min(255, (int)(result.r + base.r * factor));
        result.g = std::min(255, (int)(result.g + base.g * factor));
        result.b = std::min(255, (int)(result.b + base.b * factor));
        ++count;
    }

    if (count == 0) return RGB(0, 0, 0);

    result.r /= count;
    result.g /= count;
    result.b /= count;

    return result;
}

}
