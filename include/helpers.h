#ifndef I3GUESSTEMATOR_HELPERS_H
#define I3GUESSTEMATOR_HELPERS_H

#include <cstdint>
#include <string_view>
#include <string>

static uint64_t getUInt64_tAfterKey(std::string_view str, std::string_view key) {
    std::string::size_type first = str.find(key);
    if (first == std::string::npos) {
        return 0;
    }
    first += key.size();

    return std::strtoul(str.begin() + first, nullptr, 10);
}

#endif //I3GUESSTEMATOR_HELPERS_H
