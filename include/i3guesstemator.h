/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_I3GUESSTEMATOR_H
#define I3GUESSTEMATOR_I3GUESSTEMATOR_H

#include <memory>
#include <utility>
#include <thread>
#include <chrono>

#include "yaml-cpp/yaml.h"
#include "writer.h"
#include "i3barWriter.h"
#include "batteryGenerator.h"
#include "brightnessGenerator.h"
#include "cpuGenerator.h"
#include "ramGenerator.h"
#include "timeGenerator.h"

class I3Guesstemator {
    int64_t timeToSleep;
    std::unique_ptr<Writer> writer;
    bool work = true;

    void initWriter(const YAML::Node& config) {
        auto value = config["writer"].as<std::string>();
        if (value == "i3bar") {
            writer = std::make_unique<I3barWriter>(config);
        } else {
            throw std::runtime_error("failed to init the writer");
        }
    }

    void pushGenerator(std::string_view name, const YAML::Node& config) const {
        if (name.contains("battery")) {
            writer->pushBack(std::make_unique<BatteryGenerator>(config));
        } else if (name.contains("brightness")) {
            writer->pushBack(std::make_unique<BrightnessGenerator>(config));
        } else if (name.contains("cpu")) {
            writer->pushBack(std::make_unique<CpuGenerator>(config));
        } else if (name.contains("ram")) {
            writer->pushBack(std::make_unique<RamGenerator>(config));
        } else if (name.contains("time")) {
            writer->pushBack(std::make_unique<TimeGenerator>(config));
        }
    }

    void initGenerators(const YAML::Node& config) const {
        auto arr = config["generatorList"].as<std::vector<std::string>>();
        for (const auto &str: arr) {
            pushGenerator(str, config);
        }
    }

public:
    I3Guesstemator() : timeToSleep(1000), writer(std::make_unique<I3barWriter>()) {
        writer->pushBack(std::make_unique<TimeGenerator>(YAML::Node()));
    }

    explicit I3Guesstemator(const YAML::Node &config) {
        timeToSleep = config["updateInterval"].as<int64_t>();
        initWriter(config);
        initGenerators(config);
    }

    void run() const {
        while (work) {
            writer->update();
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
        }
    }
};

#endif //I3GUESSTEMATOR_I3GUESSTEMATOR_H
