/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_TIMEGENERATOR_H
#define I3GUESSTEMATOR_TIMEGENERATOR_H

#include <chrono>
#include <format>

#include "elementGenerator.h"
#include "element.h"

class TimeGenerator : public ElementGenerator {
public:
    explicit TimeGenerator(const YAML::Node &config) : ElementGenerator("time", config) {}

    Element getElement() override {
        std::stringstream ss;
        ss << prefix;
        ss << std::format("{:%d-%m-%Y %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(
                std::chrono::current_zone()->to_local(std::chrono::system_clock::now())));
        return Element{ss.str()};
    }
};

#endif //I3GUESSTEMATOR_TIMEGENERATOR_H
