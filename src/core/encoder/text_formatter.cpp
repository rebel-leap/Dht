#include <string>

namespace Dht {

std::string formatText(const std::string& text, int width) {
    std::string result;
    int count = 0;

    for (char c : text) {
        result += c;
        if (c == ' ') {
            ++count;
            if (count >= width) {
                result += '\n';
                count = 0;
            }
        }
    }

    return result;
}

}
