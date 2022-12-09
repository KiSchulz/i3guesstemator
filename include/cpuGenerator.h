#ifndef I3GUESSTEMATOR_CPUGENERATOR_H
#define I3GUESSTEMATOR_CPUGENERATOR_H

#include <array>
#include <sstream>
#include <numeric>
#include <iomanip>
#include <cmath>

#include "elementGenerator.h"
#include "element.h"
#include "updatingFileBuffer.h"

class CpuGenerator : public ElementGenerator {
    UpdatingFileBuffer stat;
    std::array<uint64_t, 10> cpu;

public:
    CpuGenerator() : stat("/proc/stat"), cpu(getCpuTimes()) {
    }

    Element getElement() override {
        std::array<uint64_t, 10> nCpu = getCpuTimes();
        std::array<uint64_t, 10> delta = nCpu;

        for (uint64_t i = 0; i < cpu.size(); i++) {
            delta[i] -= cpu[i];
        }
        uint64_t total = std::accumulate(delta.begin(), delta.end(), 0ul);

        std::stringstream ss;
        ss << (int) std::lround((1.0 - ((float) delta[3] / (float) total)) * 100.0) << "%";

        cpu = nCpu;

        return Element{ss.str()};
    }

private:
    std::array<uint64_t, 10> getCpuTimes() {
        std::array<uint64_t, 10> ret{};
        std::string_view file = stat.getContent();
        const char *startPtr = file.begin() + 5;
        char *endPtr;

        for (auto &i: ret) {
            i = strtoul(startPtr, &endPtr, 10);
            startPtr = endPtr;
        }

        return ret;
    }
};

#endif //I3GUESSTEMATOR_CPUGENERATOR_H
