#include <string>
#include "dht/dht.hpp"
#include "core/io/file_utils.hpp"
#include "core/decoder/text_to_pixel.cpp"
#include "core/decoder/png_writer.cpp"
#include "core/decoder/jpeg_writer.cpp"
#include "core/decoder/bmp_writer.cpp"
#include "core/compression/xz_decompressor.cpp"
#include "core/crypto/aes_gcm_decryptor.cpp"

namespace Dht {

void decryptImage(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    std::vector<uint8_t> encrypted = readFile(inputPath);
    std::vector<uint8_t> decrypted = decryptData(encrypted, password);
    std::string textData = decompressXZ(decrypted);

    int width = 0, height = 0;
    std::vector<RGB> pixels = textToPixel(textData, width, height);

    std::string ext = getFileExtension(outputPath);
    if (ext == "png") {
        writePNG(outputPath, pixels, width, height);
    } else if (ext == "jpg" || ext == "jpeg") {
        writeJPEG(outputPath, pixels, width, height);
    } else if (ext == "bmp") {
        writeBMP(outputPath, pixels, width, height);
    } else {
        throw UnsupportedFormatError(ext);
    }
}

}
