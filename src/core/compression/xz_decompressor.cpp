#include <lzma.h>
#include <vector>
#include <string>
#include "dht/exceptions.hpp"

namespace Dht {

std::string decompressXZ(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return std::string();
    }

    lzma_stream stream = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_stream_decoder(&stream, UINT64_MAX, LZMA_CONCATENATED);
    if (ret != LZMA_OK) {
        throw DhtError("Failed to initialize XZ decoder");
    }

    std::vector<uint8_t> input = data;
    std::vector<uint8_t> output;
    output.reserve(input.size() * 2);

    stream.next_in = input.data();
    stream.avail_in = input.size();

    std::vector<uint8_t> outBuffer(8192);

    while (ret != LZMA_STREAM_END) {
        stream.next_out = outBuffer.data();
        stream.avail_out = outBuffer.size();

        ret = lzma_code(&stream, LZMA_RUN);

        if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
            lzma_end(&stream);
            throw DhtError("XZ decompression failed");
        }

        size_t bytesWritten = outBuffer.size() - stream.avail_out;
        output.insert(output.end(), outBuffer.data(), outBuffer.data() + bytesWritten);
    }

    lzma_end(&stream);
    return std::string(output.begin(), output.end());
}

}
