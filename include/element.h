#ifndef I3GUESSTEMATOR_ELEMENT_H
#define I3GUESSTEMATOR_ELEMENT_H

#include <string>

#include <cstdint>
#include <utility>

enum class Alignment {
    left, center, right
};

struct Color {
    uint8_t r, g, b;
};

struct Element {
    std::string text;
    Color textColor = {0xff, 0xff, 0xff};
    Color backgroundColor = {0x00, 0x00, 0x00};
    Alignment alignment = Alignment::center;

    explicit Element(std::string text) : text(std::move(text)) {}
};

#endif //I3GUESSTEMATOR_ELEMENT_H
