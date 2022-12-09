#include "i3guesstemator.h"
#include "i3barWriter.h"
#include "timeGenerator.h"
#include "batteryGenerator.h"
#include "ramGenerator.h"
#include "cpuGenerator.h"

#include <memory>

int main() {
    //TODO: add a config file reader

    I3barWriter i3BarWriter;

    i3BarWriter.pushBack(std::make_shared<BatteryGenerator>());
    i3BarWriter.pushBack(std::make_shared<CpuGenerator>());
    i3BarWriter.pushBack(std::make_shared<RamGenerator>());
    i3BarWriter.pushBack(std::make_shared<TimeGenerator>());

    I3Guesstemator i3Guesstemator(std::make_shared<I3barWriter>(i3BarWriter), 5);
    i3Guesstemator.run();

    return 0;
}
