#ifndef DHT_COLOR_TYPES_HPP
#define DHT_COLOR_TYPES_HPP

#include <cstdint>
#include <unordered_map>

namespace Dht {

struct RGB {
    uint8_t r, g, b;
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
};

static const std::unordered_map<char, RGB> BASE_COLORS = {
    {'R', {255, 0, 0}},
    {'G', {0, 255, 0}},
    {'B', {0, 0, 255}}
};

}

#endif
