#include <vector>
#include <string>
#include "dht/dht.hpp"
#include "core/crypto/aes_gcm_decryptor.cpp"

namespace Dht {

std::vector<uint8_t> decryptData(const std::vector<uint8_t>& encryptedData, const std::string& password) {
    return Dht::decryptData(encryptedData, password);
}

}
