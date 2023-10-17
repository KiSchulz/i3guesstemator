/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_RAMGENERATOR_H
#define I3GUESSTEMATOR_RAMGENERATOR_H

#include <fstream>
#include <format>

#include "helpers.h"
#include "elementGenerator.h"
#include "element.h"
#include "updatingFileBuffer.h"

class RamGenerator : public ElementGenerator {
    UpdatingFileBuffer memInfo;
public:
    explicit RamGenerator(const YAML::Node &config) : ElementGenerator("ram", config), memInfo("/proc/meminfo") {}

    Element getElement() override {
        Ram ram{memInfo.getContent()};
        std::stringstream ss;
        ss << prefix;
        ss << std::format("{:05.2f}/{:05.2f}GiB",
                          Ram::KiBToGiB(ram.memTotal - ram.memFree - ram.buffers - ram.cached),
                          Ram::KiBToGiB(ram.memTotal));

        return Element(ss.str());
    }

private:
    struct Ram {
        // everything in the unit(kB) the file provides
        uint64_t memTotal;
        uint64_t memFree;
        uint64_t buffers;
        uint64_t cached;

        explicit Ram(std::string_view memInfo) : memTotal(getUInt64_tAfterKey(memInfo, "MemTotal:")),
                                                 memFree(getUInt64_tAfterKey(memInfo, "MemFree:")),
                                                 buffers(getUInt64_tAfterKey(memInfo, "Buffers:")),
                                                 cached(getUInt64_tAfterKey(memInfo, "Cached:")) {}

        static double KiBToGiB(uint64_t n) {
            return (double) n / (double) (1 << 20);
        }
    };
};

#endif //I3GUESSTEMATOR_RAMGENERATOR_H
