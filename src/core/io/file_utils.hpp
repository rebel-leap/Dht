#ifndef DHT_FILE_UTILS_HPP
#define DHT_FILE_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace Dht {

std::vector<uint8_t> readFile(const std::string& path);
void writeFile(const std::string& path, const std::vector<uint8_t>& data);
bool fileExists(const std::string& path);
std::string getFileExtension(const std::string& path);

}

#endif
