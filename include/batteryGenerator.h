/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_BATTERYGENERATOR_H
#define I3GUESSTEMATOR_BATTERYGENERATOR_H

#include <chrono>
#include <ctime>
#include <format>
#include <fstream>
#include <string_view>

#include "battery.h"
#include "elementGenerator.h"
#include "helpers.h"
#include "updatingAverage.h"
#include "updatingFileBuffer.h"
#include "yaml-cpp/yaml.h"

class BatteryGenerator : public ElementGenerator {
  UpdatingFileBuffer uevent;
  UpdatingAverage<uint64_t> timeAvg;
  Battery battery{};

  UpdatingFileBuffer initUpdatingFileBuffer() {
      const YAML::Node &pathNode = generatorConfig["path"];
      if(pathNode.IsDefined()){
        return UpdatingFileBuffer{pathNode.as<std::string>()};
      }

      return UpdatingFileBuffer{"/sys/class/power_supply/BAT0/uevent"};
  }

  UpdatingAverage<uint64_t> initTimeAvg() {
    if (generatorConfig.IsDefined()) {
      const YAML::Node &numIntervals = generatorConfig["numIntervals"];
      if (numIntervals.IsDefined()) {
        return UpdatingAverage<uint64_t>{numIntervals.as<uint64_t>()};
      }
    }
    return UpdatingAverage<uint64_t>{5 * 60};
  }

public:
  explicit BatteryGenerator(std::string_view name, const YAML::Node &config)
      : ElementGenerator(name, config), uevent(initUpdatingFileBuffer()), timeAvg(initTimeAvg()) {}

  Element getElement() override {
    const Battery nBattery = Battery{uevent.getContent()};
    if (nBattery.status != battery.status) {
      timeAvg = UpdatingAverage<uint64_t>{timeAvg.getNumEntries()};
    }
    battery = nBattery;
    timeAvg.push(battery.getTimeLeft());

    std::stringstream ss;
    ss.precision(3);
    ss << prefix;

    // battery capacity in percent
    ss << battery.capacity << "% ";

    // power draw in watts
    if (battery.status == -1) {
      ss << "-";
    }

    ss << std::format("{:05.2f} W ", battery.getPowerDraw());

    // estimated time of death
    // calculating the time
    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);
    tm.tm_sec += (int)timeAvg.getAverage();
    std::mktime(&tm);
    ss << "ETD=" << std::put_time(&tm, "%H:%M") << " ";

    // time left
    std::chrono::hh_mm_ss hhMmSs{std::chrono::seconds{timeAvg.getAverage()}};
    ss << std::format("T={:02}:{:02}h", hhMmSs.hours().count(), hhMmSs.minutes().count());

    return Element{ss.str(), battery.capacity < 20 ? -1 : 0};
  }
};

#endif // I3GUESSTEMATOR_BATTERYGENERATOR_H
