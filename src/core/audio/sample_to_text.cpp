#include <string>
#include <vector>
#include <cmath>

namespace Dht {

std::string sampleToText(const std::vector<double>& samples, int samplesPerSecond) {
    std::string result;
    int step = samples.size() / samplesPerSecond;
    if (step < 1) step = 1;

    for (size_t i = 0; i < samples.size(); i += step) {
        double amplitude = fabs(samples[i]) * 100.0;
        if (amplitude > 100.0) amplitude = 100.0;
        result += "A(" + std::to_string((int)amplitude) + ")\n";
    }

    return result;
}

}
