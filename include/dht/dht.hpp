#ifndef DHT_HPP
#define DHT_HPP

#include "version.hpp"
#include "exceptions.hpp"
#include "config.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace Dht {

void encryptImage(const std::string& inputPath, const std::string& outputPath, const std::string& password);
void decryptImage(const std::string& inputPath, const std::string& outputPath, const std::string& password);
void encryptVideo(const std::string& inputPath, const std::string& outputPath, const std::string& password, int fps = DEFAULT_FPS, int audioSamplesPerSecond = DEFAULT_AUDIO_SAMPLES);
void decryptVideo(const std::string& inputPath, const std::string& outputPath, const std::string& password);
void encryptAudio(const std::string& inputPath, const std::string& outputPath, const std::string& password, int samplesPerSecond = DEFAULT_AUDIO_SAMPLES);
void decryptAudio(const std::string& inputPath, const std::string& outputPath, const std::string& password);
std::vector<uint8_t> encryptData(const std::vector<uint8_t>& data, const std::string& password);
std::vector<uint8_t> decryptData(const std::vector<uint8_t>& encryptedData, const std::string& password);
const char* version();
bool isValidFile(const std::string& path);

}

#endif
