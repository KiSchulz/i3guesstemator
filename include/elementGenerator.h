#ifndef I3GUESSTEMATOR_ELEMENTGENERATOR_H
#define I3GUESSTEMATOR_ELEMENTGENERATOR_H

#include "element.h"

class ElementGenerator {
public:
    virtual ~ElementGenerator() = default;
    virtual Element getElement() = 0;
};

#endif //I3GUESSTEMATOR_ELEMENTGENERATOR_H
