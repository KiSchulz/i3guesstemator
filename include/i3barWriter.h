/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_I3BARWRITER_H
#define I3GUESSTEMATOR_I3BARWRITER_H

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "element.h"
#include "elementGenerator.h"
#include "writer.h"
#include "yaml-cpp/yaml.h"

struct Color {
  uint8_t r = 255, g = 255, b = 255;
};

class I3barWriter : public Writer {
  bool useColor = true;
  Color positive, negative, neutral;

public:
  I3barWriter() {
    // init the connection to i3Bar
    std::cout << "{ \"version\": 1 }\n[" << std::endl;
  }

  explicit I3barWriter(const YAML::Node &config) : I3barWriter() {
    const YAML::Node &i3barNode = config["i3bar"];
    if (!i3barNode.IsDefined()) {
      return;
    }

    const YAML::Node &useColorNode = i3barNode["useColor"];
    if (useColorNode.IsDefined()) {
      useColor = useColorNode.as<bool>();
    }

    if (useColor) {
      const YAML::Node &positiveColorNode = i3barNode["positiveColor"];
      if (positiveColorNode.IsDefined()) {
        positive = colorFromNode(positiveColorNode);
      }

      const YAML::Node &negativeColorNode = i3barNode["negativeColor"];
      if (negativeColorNode.IsDefined()) {
        negative = colorFromNode(negativeColorNode);
      }

      const YAML::Node &neutralColorNode = i3barNode["neutralColor"];
      if (neutralColorNode.IsDefined()) {
        neutral = colorFromNode(neutralColorNode);
      }
    }
  }

  // update all elements and output them to i3bar
  void update() override {
    // start the update
    std::cout << "[\n";

    // print every element except the last one because of formatting
    for (std::size_t i = 0; i < elementGenerators.size() - 1; i++) {
      std::cout << elementToString(elementGenerators[i]->getElement()) << ",\n";
    }
    // print the last element without a colon at the end
    std::cout << elementToString(elementGenerators.back()->getElement()) << "\n";

    // end of the update
    std::cout << "]," << std::endl;
  }

  void pushBack(std::unique_ptr<ElementGenerator> elementGenerator) override {
    elementGenerators.push_back(std::move(elementGenerator));
  }

private:
  [[nodiscard]] std::string elementToString(const Element &element) const {
    std::stringstream ss;
    ss << "{\n";

    ss << R"("full_text": ")" << element.text << "\",\n";

    if (useColor) {
      if (element.indicatorLevel < 0) {
        ss << R"("color": "#)" << std::setw(2) << std::setfill('0') << std::hex << (int)negative.r << std::setw(2)
           << std::setfill('0') << std::hex << (int)negative.g << std::setw(2) << std::setfill('0') << std::hex
           << (int)negative.b << "\"\n";
      } else if (element.indicatorLevel == 0) {
        ss << R"("color": "#)" << std::setw(2) << std::setfill('0') << std::hex << (int)neutral.r << std::setw(2)
           << std::setfill('0') << std::hex << (int)neutral.g << std::setw(2) << std::setfill('0') << std::hex
           << (int)neutral.b << "\"\n";
      } else if (element.indicatorLevel > 0) {
        ss << R"("color": "#)" << std::setw(2) << std::setfill('0') << std::hex << (int)positive.r << std::setw(2)
           << std::setfill('0') << std::hex << (int)positive.g << std::setw(2) << std::setfill('0') << std::hex
           << (int)positive.b << "\"\n";
      }
    }
    // TODO add all the other options

    ss << "}";
    return ss.str();
  }

  static Color colorFromNode(const YAML::Node &node) {
    auto vec = node.as<std::vector<uint8_t>>();
    if (vec.size() != 3) {
      throw std::runtime_error("invalid color format in the config file!");
    }
    return Color{vec[0], vec[1], vec[2]};
  }
};

#endif // I3GUESSTEMATOR_I3BARWRITER_H
