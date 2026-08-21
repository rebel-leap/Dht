#ifndef DHT_EXCEPTIONS_HPP
#define DHT_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace Dht {

class DhtError : public std::runtime_error {
public:
    explicit DhtError(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidPasswordError : public DhtError {
public:
    InvalidPasswordError() : DhtError("Invalid password") {}
};

class WeakPasswordError : public DhtError {
public:
    WeakPasswordError() : DhtError("Password must be at least 32 characters long") {}
};

class CorruptedFileError : public DhtError {
public:
    CorruptedFileError() : DhtError("File is corrupted or invalid") {}
};

class DecryptionFailedError : public DhtError {
public:
    DecryptionFailedError() : DhtError("Decryption failed") {}
};

class UnsupportedFormatError : public DhtError {
public:
    explicit UnsupportedFormatError(const std::string& format)
        : DhtError("Unsupported format: " + format) {}
};

class FileNotFoundError : public DhtError {
public:
    explicit FileNotFoundError(const std::string& path)
        : DhtError("File not found: " + path) {}
};

}

#endif
