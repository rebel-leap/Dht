#include <lzma.h>
#include <vector>
#include <string>
#include "dht/exceptions.hpp"

namespace Dht {

std::vector<uint8_t> compressXZ(const std::string& data) {
    if (data.empty()) {
        return std::vector<uint8_t>();
    }

    lzma_stream stream = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_easy_encoder(&stream, 9, LZMA_CHECK_CRC64);
    if (ret != LZMA_OK) {
        throw DhtError("Failed to initialize XZ encoder");
    }

    std::vector<uint8_t> input(data.begin(), data.end());
    std::vector<uint8_t> output;
    output.reserve(input.size() / 2);

    stream.next_in = input.data();
    stream.avail_in = input.size();

    std::vector<uint8_t> outBuffer(8192);

    while (ret != LZMA_STREAM_END) {
        stream.next_out = outBuffer.data();
        stream.avail_out = outBuffer.size();

        ret = lzma_code(&stream, (stream.avail_in == 0) ? LZMA_FINISH : LZMA_RUN);

        if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
            lzma_end(&stream);
            throw DhtError("XZ compression failed");
        }

        size_t bytesWritten = outBuffer.size() - stream.avail_out;
        output.insert(output.end(), outBuffer.data(), outBuffer.data() + bytesWritten);
    }

    lzma_end(&stream);
    return output;
}

}
