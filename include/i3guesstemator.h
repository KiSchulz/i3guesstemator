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

    const std::map<std::string, std::unique_ptr<ElementGenerator> (*)(std::string_view,
                                                                             const YAML::Node &)> elementGeneratorsMap{
            {"battery",    &ElementGenerator::createElement<BatteryGenerator>},
            {"brightness", &ElementGenerator::createElement<BrightnessGenerator>},
            {"cpu",        &ElementGenerator::createElement<CpuGenerator>},
            {"ram",        &ElementGenerator::createElement<RamGenerator>},
            {"time",       &ElementGenerator::createElement<TimeGenerator>},
    };

    void initWriter(const YAML::Node &config) {
        auto value = config["writer"].as<std::string>();
        if (value == "i3bar") {
            writer = std::make_unique<I3barWriter>(config);
        } else {
            throw std::runtime_error("Failed to init the writer!");
        }
    }

    void pushGenerator(std::string_view name, const YAML::Node &config) const {
        try {
            if (elementGeneratorsMap.contains(name.cbegin())) {
                writer->pushBack(elementGeneratorsMap.at(name.cbegin())(name, config));
            } else {
                std::cerr << "Error: " << name << " is not a valid element generator!" << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to create " << name << " with error: " << e.what() << std::endl;
        }
    }

    void initGenerators(const YAML::Node &config) const {
        auto arr = config["generatorList"].as<std::vector<std::string>>();
        for (const auto &name: arr) {
            pushGenerator(name, config);
        }
    }

public:

    I3Guesstemator() : timeToSleep(1000), writer(std::make_unique<I3barWriter>()) {
        std::string name = "time";
        writer->pushBack(elementGeneratorsMap.at(name)(name, YAML::Node()));
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
