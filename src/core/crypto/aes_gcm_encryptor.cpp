#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <vector>
#include "dht/config.hpp"
#include "dht/exceptions.hpp"

namespace Dht {

static void validatePassword(const std::string& password) {
    if (password.length() < MIN_PASSWORD_LENGTH) {
        throw WeakPasswordError();
    }
}

std::vector<uint8_t> encryptData(const std::vector<uint8_t>& data, const std::string& password) {
    validatePassword(password);

    std::vector<uint8_t> key(AES_KEY_LENGTH);
    size_t copyLen = std::min(password.length(), (size_t)AES_KEY_LENGTH);
    std::memcpy(key.data(), password.data(), copyLen);

    std::vector<uint8_t> iv(AES_IV_LENGTH);
    if (RAND_bytes(iv.data(), iv.size()) != 1) {
        throw DhtError("Failed to generate IV");
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw DhtError("Failed to create cipher context");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key.data(), iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DhtError("Failed to initialize encryption");
    }

    std::vector<uint8_t> ciphertext(data.size() + 16);
    int len = 0;
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, data.data(), data.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DhtError("Encryption failed");
    }
    int ciphertextLen = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DhtError("Encryption finalization failed");
    }
    ciphertextLen += len;
    ciphertext.resize(ciphertextLen);

    std::vector<uint8_t> tag(AES_TAG_LENGTH);
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DhtError("Failed to get GCM tag");
    }

    EVP_CIPHER_CTX_free(ctx);

    std::vector<uint8_t> result;
    result.reserve(iv.size() + tag.size() + ciphertext.size());
    result.insert(result.end(), iv.begin(), iv.end());
    result.insert(result.end(), tag.begin(), tag.end());
    result.insert(result.end(), ciphertext.begin(), ciphertext.end());

    return result;
}

}
