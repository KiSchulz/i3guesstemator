//
// Created by kis on 26.11.22.
//

#ifndef I3GUESSTEMATOR_WRITER_H
#define I3GUESSTEMATOR_WRITER_H

#include <vector>
#include <memory>
#include <iostream>

#include "elementGenerator.h"

class Writer {
protected:
    std::vector<std::shared_ptr<ElementGenerator>> elementGenerators;

public:
    virtual ~Writer() = default;

    virtual void update() = 0;

    virtual void pushBack(const std::shared_ptr<ElementGenerator> &elementGenerator) = 0;
};

#endif //I3GUESSTEMATOR_WRITER_H
