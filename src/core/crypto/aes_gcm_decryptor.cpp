#include <openssl/evp.h>
#include <cstring>
#include <vector>
#include "dht/config.hpp"
#include "dht/exceptions.hpp"

namespace Dht {

std::vector<uint8_t> decryptData(const std::vector<uint8_t>& encryptedData, const std::string& password) {
    if (password.length() < MIN_PASSWORD_LENGTH) {
        throw WeakPasswordError();
    }

    if (encryptedData.size() < AES_IV_LENGTH + AES_TAG_LENGTH) {
        throw CorruptedFileError();
    }

    size_t offset = 0;
    std::vector<uint8_t> iv(encryptedData.begin() + offset, encryptedData.begin() + offset + AES_IV_LENGTH);
    offset += AES_IV_LENGTH;

    std::vector<uint8_t> tag(encryptedData.begin() + offset, encryptedData.begin() + offset + AES_TAG_LENGTH);
    offset += AES_TAG_LENGTH;

    std::vector<uint8_t> ciphertext(encryptedData.begin() + offset, encryptedData.end());

    std::vector<uint8_t> key(AES_KEY_LENGTH);
    size_t copyLen = std::min(password.length(), (size_t)AES_KEY_LENGTH);
    std::memcpy(key.data(), password.data(), copyLen);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw DhtError("Failed to create cipher context");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key.data(), iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DecryptionFailedError();
    }

    std::vector<uint8_t> plaintext(ciphertext.size());
    int len = 0;
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DecryptionFailedError();
    }
    int plaintextLen = len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw DecryptionFailedError();
    }

    int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    EVP_CIPHER_CTX_free(ctx);

    if (ret != 1) {
        throw InvalidPasswordError();
    }

    plaintextLen += len;
    plaintext.resize(plaintextLen);

    return plaintext;
}

}
