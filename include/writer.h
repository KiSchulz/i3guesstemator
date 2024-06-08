/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_WRITER_H
#define I3GUESSTEMATOR_WRITER_H

#include <iostream>
#include <memory>
#include <vector>

#include "elementGenerator.h"

class Writer {
protected:
  std::vector<std::unique_ptr<ElementGenerator>> elementGenerators;

public:
  virtual ~Writer() = default;

  virtual void update() = 0;

  virtual void pushBack(std::unique_ptr<ElementGenerator> elementGenerator) = 0;
};

#endif // I3GUESSTEMATOR_WRITER_H
