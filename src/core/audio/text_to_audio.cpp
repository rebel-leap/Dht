#include <string>
#include <vector>
#include <sstream>

namespace Dht {

std::vector<double> textToAudio(const std::string& text) {
    std::vector<double> samples;
    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.find("A(") != std::string::npos) {
            size_t start = line.find('(') + 1;
            size_t end = line.find(')');
            if (start != std::string::npos && end != std::string::npos) {
                int value = std::stoi(line.substr(start, end - start));
                samples.push_back(value / 100.0);
            }
        }
    }

    return samples;
}

}
