/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_BATTERYGENERATOR_H
#define I3GUESSTEMATOR_BATTERYGENERATOR_H

#include <fstream>
#include <iomanip>
#include <string_view>
#include <chrono>
#include <ctime>

#include "helpers.h"
#include "elementGenerator.h"
#include "updatingFileBuffer.h"
#include "updatingAverage.h"
#include "battery.h"

template<std::size_t avgBufferSize = 60 * 5>
class BatteryGenerator : public ElementGenerator {
    UpdatingFileBuffer uevent;
    UpdatingAverage<uint64_t, avgBufferSize> timeAvg;
    Battery battery{};
public:
    BatteryGenerator() : uevent("/sys/class/power_supply/BAT0/uevent") {}

    Element getElement() override {
        const Battery nBattery = Battery{uevent.getContent()};
        if (nBattery.status != battery.status) {
            timeAvg = UpdatingAverage<uint64_t, avgBufferSize>{};
        }
        battery = nBattery;
        timeAvg.push(battery.getTimeLeft());

        std::stringstream ss;
        ss.precision(3);
        ss << "\uf242    ";

        //battery capacity in percent
        ss << battery.capacity << "% ";

        //power draw in watts
        ss << std::setfill('0') << std::setw(2) << (double) battery.status * battery.getPowerDraw() << "W ";

        //estimated time of death
        //calculating the time
        time_t t = std::time(nullptr);
        tm tm = *std::localtime(&t);
        tm.tm_sec += (int) timeAvg.getAverage();
        std::mktime(&tm);
        ss << "ETD="
           << std::put_time(&tm, "%H:%M") << " ";

        //time left
        std::chrono::hh_mm_ss hhMmSs{std::chrono::seconds{timeAvg.getAverage()}};
        ss << "T="
           << std::setfill('0') << std::setw(2) << hhMmSs.hours().count() << ":"
           << std::setfill('0') << std::setw(2) << hhMmSs.minutes().count() << "h";

        return Element{ss.str()};
    }
};

#endif //I3GUESSTEMATOR_BATTERYGENERATOR_H
