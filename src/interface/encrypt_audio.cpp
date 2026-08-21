#include <string>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/audio/audio_reader.cpp"
#include "core/audio/sample_to_text.cpp"
#include "core/compression/xz_compressor.cpp"
#include "core/crypto/aes_gcm_encryptor.cpp"

namespace Dht {

void encryptAudio(const std::string& inputPath, const std::string& outputPath, const std::string& password, int samplesPerSecond) {
    std::vector<double> samples = readAudio(inputPath);
    std::string textData = sampleToText(samples, samplesPerSecond);
    std::vector<uint8_t> compressed = compressXZ(textData);
    std::vector<uint8_t> encrypted = encryptData(compressed, password);
    writeFile(outputPath, encrypted);
}

}
