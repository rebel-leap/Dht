#include <string>
#include <vector>
#include <sstream>

namespace Dht {

std::vector<std::string> parseText(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

}
