#include <string>
#include <vector>
#include "core/audio/sample_to_text.cpp"

namespace Dht {

std::string audioToText(const std::vector<double>& samples, int samplesPerSecond) {
    return sampleToText(samples, samplesPerSecond);
}

}
