#include <string>
#include <vector>
#include <sstream>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/decoder/text_to_pixel.cpp"
#include "core/decoder/text_parser.cpp"
#include "core/decoder/png_writer.cpp"
#include "core/decoder/jpeg_writer.cpp"
#include "core/compression/xz_decompressor.cpp"
#include "core/crypto/aes_gcm_decryptor.cpp"

namespace Dht {

void decryptVideo(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    std::vector<uint8_t> encrypted = readFile(inputPath);
    std::vector<uint8_t> decrypted = decryptData(encrypted, password);
    std::string textData = decompressXZ(decrypted);

    std::vector<std::string> parts;
    size_t audioPos = textData.find("---AUDIO_START---");
    std::string videoText;
    std::string audioText;

    if (audioPos != std::string::npos) {
        videoText = textData.substr(0, audioPos);
        audioText = textData.substr(audioPos + 18);
    } else {
        videoText = textData;
        audioText = "";
    }

    std::vector<std::string> frameTexts;
    std::string currentFrame;
    std::istringstream iss(videoText);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.empty() && !currentFrame.empty()) {
            frameTexts.push_back(currentFrame);
            currentFrame.clear();
        } else if (!line.empty()) {
            if (!currentFrame.empty()) currentFrame += "\n";
            currentFrame += line;
        }
    }
    if (!currentFrame.empty()) {
        frameTexts.push_back(currentFrame);
    }

    int width = 0, height = 0;
    std::vector<RGB> allPixels;
    std::string ext = getFileExtension(outputPath);

    for (size_t i = 0; i < frameTexts.size(); i++) {
        int w = 0, h = 0;
        std::vector<RGB> pixels = textToPixel(frameTexts[i], w, h);
        if (i == 0) {
            width = w;
            height = h;
        }
        allPixels.insert(allPixels.end(), pixels.begin(), pixels.end());
    }

    if (ext == "png") {
        writePNG(outputPath, allPixels, width, height);
    } else if (ext == "jpg" || ext == "jpeg") {
        writeJPEG(outputPath, allPixels, width, height);
    } else {
        throw UnsupportedFormatError(ext);
    }
}

}
