#include <string>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/video/frame_extractor.cpp"
#include "core/video/frame_to_text.cpp"
#include "core/video/audio_extractor.cpp"
#include "core/video/audio_to_text.cpp"
#include "core/video/video_text_joiner.cpp"
#include "core/compression/xz_compressor.cpp"
#include "core/crypto/aes_gcm_encryptor.cpp"

namespace Dht {

void encryptVideo(const std::string& inputPath, const std::string& outputPath, const std::string& password, int fps, int audioSamplesPerSecond) {
    int width = 0, height = 0;
    std::vector<std::vector<RGB>> frames = extractFrames(inputPath, width, height, fps);

    std::vector<std::string> frameTexts;
    for (const auto& frame : frames) {
        frameTexts.push_back(frameToText(frame, width, height));
    }

    std::vector<double> audioSamples = extractAudio(inputPath);
    std::string audioText = audioToText(audioSamples, audioSamplesPerSecond);

    std::string textData = joinVideoText(frameTexts, audioText);
    std::vector<uint8_t> compressed = compressXZ(textData);
    std::vector<uint8_t> encrypted = encryptData(compressed, password);
    writeFile(outputPath, encrypted);
}

}
