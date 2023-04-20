/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_ELEMENTGENERATOR_H
#define I3GUESSTEMATOR_ELEMENTGENERATOR_H

#include "element.h"

class ElementGenerator {
public:
    virtual ~ElementGenerator() = default;
    virtual Element getElement() = 0;
};

#endif //I3GUESSTEMATOR_ELEMENTGENERATOR_H
