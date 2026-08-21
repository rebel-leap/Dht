#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace Dht {

class MemoryStream {
public:
    MemoryStream() : data_(), position_(0) {}
    explicit MemoryStream(const std::vector<uint8_t>& data) : data_(data), position_(0) {}

    void write(const uint8_t* buffer, size_t size) {
        data_.insert(data_.end(), buffer, buffer + size);
    }

    void read(uint8_t* buffer, size_t size) {
        if (position_ + size > data_.size()) {
            throw std::out_of_range("MemoryStream: read out of range");
        }
        std::memcpy(buffer, data_.data() + position_, size);
        position_ += size;
    }

    size_t size() const { return data_.size(); }
    size_t position() const { return position_; }
    void seek(size_t pos) { position_ = pos; }
    const std::vector<uint8_t>& data() const { return data_; }

private:
    std::vector<uint8_t> data_;
    size_t position_;
};

}
