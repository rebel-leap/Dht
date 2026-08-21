#include <vector>
#include <string>
#include "dht/dht.hpp"
#include "core/crypto/aes_gcm_encryptor.cpp"

namespace Dht {

std::vector<uint8_t> encryptData(const std::vector<uint8_t>& data, const std::string& password) {
    return Dht::encryptData(data, password);
}

}
