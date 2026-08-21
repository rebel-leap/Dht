#include <string>
#include <vector>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/audio/text_to_audio.cpp"
#include "core/compression/xz_decompressor.cpp"
#include "core/crypto/aes_gcm_decryptor.cpp"

namespace Dht {

void decryptAudio(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    std::vector<uint8_t> encrypted = readFile(inputPath);
    std::vector<uint8_t> decrypted = decryptData(encrypted, password);
    std::string textData = decompressXZ(decrypted);
    std::vector<double> samples = textToAudio(textData);

    (void)samples;
    throw DhtError("Audio decoding not implemented yet");
}

}
