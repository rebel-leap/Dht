#include <string>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/color/color_types.hpp"
#include "core/encoder/png_reader.cpp"
#include "core/encoder/jpeg_reader.cpp"
#include "core/encoder/bmp_reader.cpp"
#include "core/encoder/pixel_to_text.cpp"
#include "core/compression/xz_compressor.cpp"
#include "core/crypto/aes_gcm_encryptor.cpp"

namespace Dht {

void encryptImage(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    std::string ext = getFileExtension(inputPath);
    int width = 0, height = 0;
    std::vector<RGB> pixels;

    if (ext == "png") {
        pixels = readPNG(inputPath, width, height);
    } else if (ext == "jpg" || ext == "jpeg") {
        pixels = readJPEG(inputPath, width, height);
    } else if (ext == "bmp") {
        pixels = readBMP(inputPath, width, height);
    } else {
        throw UnsupportedFormatError(ext);
    }

    std::string textData = pixelToText(pixels, width, height);
    std::vector<uint8_t> compressed = compressXZ(textData);
    std::vector<uint8_t> encrypted = encryptData(compressed, password);
    writeFile(outputPath, encrypted);
}

}
