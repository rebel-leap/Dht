#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace Dht {

static std::string getTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void logInfo(const std::string& msg) {
    std::cerr << "[INFO] " << getTime() << " " << msg << std::endl;
}

void logError(const std::string& msg) {
    std::cerr << "[ERROR] " << getTime() << " " << msg << std::endl;
}

void logDebug(const std::string& msg) {
    std::cerr << "[DEBUG] " << getTime() << " " << msg << std::endl;
}

}
