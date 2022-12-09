/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_TIMEGENERATOR_H
#define I3GUESSTEMATOR_TIMEGENERATOR_H

#include <ctime>
#include <iomanip>
#include <sstream>

#include "elementGenerator.h"
#include "element.h"

class TimeGenerator : public ElementGenerator {
public:
    Element getElement() override {
        time_t t = std::time(nullptr);
        tm tm = *std::localtime(&t);

        std::stringstream ss;

        ss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");

        return Element{ss.str()};
    }
};

#endif //I3GUESSTEMATOR_TIMEGENERATOR_H
