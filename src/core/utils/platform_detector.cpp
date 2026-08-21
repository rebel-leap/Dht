#include <string>

#ifdef _WIN32
#define PLATFORM "Windows"
#elif __APPLE__
#define PLATFORM "macOS"
#elif __linux__
#define PLATFORM "Linux"
#else
#define PLATFORM "Unknown"
#endif

namespace Dht {

std::string getPlatform() {
    return PLATFORM;
}

bool isWindows() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

bool isMacOS() {
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
}

bool isLinux() {
#ifdef __linux__
    return true;
#else
    return false;
#endif
}

}
