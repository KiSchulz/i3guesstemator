/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_RAMGENERATOR_H
#define I3GUESSTEMATOR_RAMGENERATOR_H

#include <format>
#include <fstream>

#include "element.h"
#include "elementGenerator.h"
#include "helpers.h"
#include "updatingFileBuffer.h"

class RamGenerator : public ElementGenerator {
  UpdatingFileBuffer memInfo;

public:
  explicit RamGenerator(std::string_view name, const YAML::Node &config)
      : ElementGenerator(name, config), memInfo("/proc/meminfo") {}

  Element getElement() override {
    Ram ram{memInfo.getContent()};
    double usedMemory = Ram::KiBToGiB(ram.memTotal - ram.memFree - ram.buffers - ram.cached);
    double totalMemory = Ram::KiBToGiB(ram.memTotal);

    std::stringstream ss;
    ss << prefix;
    ss << std::format("{:05.2f}/{:05.2f}GiB", usedMemory, totalMemory);

    return Element(ss.str(), usedMemory / totalMemory > 0.8 ? -1 : 0);
  }

private:
  struct Ram {
    // everything in the unit(kB) the file provides
    uint64_t memTotal;
    uint64_t memFree;
    uint64_t buffers;
    uint64_t cached;

    explicit Ram(std::string_view memInfo)
        : memTotal(getUInt64_tAfterKey(memInfo, "MemTotal:")), memFree(getUInt64_tAfterKey(memInfo, "MemFree:")),
          buffers(getUInt64_tAfterKey(memInfo, "Buffers:")), cached(getUInt64_tAfterKey(memInfo, "Cached:")) {}

    static double KiBToGiB(uint64_t n) { return (double)n / (double)(1 << 20); }
  };
};

#endif // I3GUESSTEMATOR_RAMGENERATOR_H
