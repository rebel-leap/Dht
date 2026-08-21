#include <string>
#include <vector>

namespace Dht {

std::string joinVideoText(const std::vector<std::string>& frames, const std::string& audioText) {
    std::string result;

    for (size_t i = 0; i < frames.size(); i++) {
        result += frames[i];
        if (i < frames.size() - 1) {
            result += "\n\n";
        }
    }

    if (!audioText.empty()) {
        result += "\n---AUDIO_START---\n";
        result += audioText;
    }

    return result;
}

}
