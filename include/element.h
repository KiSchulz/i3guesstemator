/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_ELEMENT_H
#define I3GUESSTEMATOR_ELEMENT_H

#include <string>

#include <cstdint>
#include <utility>

enum class Alignment { left, center, right };

struct Element {
  std::string text;
  int indicatorLevel = 0;
  Alignment alignment = Alignment::center;

  explicit Element(std::string _text, int _indicatorLevel = 0)
      : text(std::move(_text)), indicatorLevel(_indicatorLevel) {}
};

#endif // I3GUESSTEMATOR_ELEMENT_H
