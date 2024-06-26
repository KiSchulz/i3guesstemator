/*
 * Copyright (c) 20.12.22 15:49 Kilian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_BRIGHTNESSGENERATOR_H
#define I3GUESSTEMATOR_BRIGHTNESSGENERATOR_H

#include <filesystem>
#include <format>
#include <fstream>
#include <string>

#include "elementGenerator.h"
#include "updatingFileBuffer.h"

class BrightnessGenerator : public ElementGenerator {
  const uint64_t maxBrightness;
  UpdatingFileBuffer brightness;

  static std::filesystem::path getPathToBacklightDir() {
    std::filesystem::path basePath{"/sys/class/backlight/"};
    std::filesystem::directory_iterator backlightDir{basePath};

    return backlightDir->path();
  }

  static uint64_t getMaxBrightness() {
    std::ifstream ifMaxBrightness{getPathToBacklightDir() / "max_brightness"};
    uint64_t ret;
    ifMaxBrightness >> ret;
    return ret;
  }

public:
  explicit BrightnessGenerator(std::string_view name, const YAML::Node &config)
      : ElementGenerator(name, config), maxBrightness(getMaxBrightness()),
        brightness(getPathToBacklightDir() / "brightness") {}

  Element getElement() override {
    uint64_t currentBrightness = std::strtoul(brightness.getContent().begin(), nullptr, 10);

    std::stringstream ss;
    ss << prefix;
    ss << std::format("{:03}%", currentBrightness * 100 / maxBrightness);

    return Element(ss.str());
  }
};

#endif // I3GUESSTEMATOR_BRIGHTNESSGENERATOR_H
