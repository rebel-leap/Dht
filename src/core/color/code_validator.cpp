#include <string>
#include "color_types.hpp"

namespace Dht {

bool isValidCode(const std::string& code) {
    if (code.empty()) return false;

    for (char c : code) {
        if (BASE_COLORS.find(c) == BASE_COLORS.end() && c != 'L' && c != 'D') {
            return false;
        }
    }

    return true;
}

}
