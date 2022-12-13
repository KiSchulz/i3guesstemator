/*
 * Copyright (c) 13.12.22 13:20 Kiian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_BATTERY_H
#define I3GUESSTEMATOR_BATTERY_H

#include <string_view>
#include <cstdint>

#include "helpers.h"

struct Battery {
    enum Status : int64_t {
        charging = 1, discharging = -1, unknown = 0, full
    };

    Status status;
    uint64_t voltageMinDesign;
    uint64_t voltageNow;
    uint64_t currentNow;
    uint64_t chargeFullDesign;
    uint64_t chargeFull;
    uint64_t chargeNow;
    uint64_t capacity;

    Battery() = default;

    explicit Battery(std::string_view config) : status(initStatus(config)),
                                                voltageMinDesign(
                                                        getUInt64_tAfterKey(config,
                                                                            "POWER_SUPPLY_VOLTAGE_MIN_DESIGN=")),
                                                voltageNow(getUInt64_tAfterKey(config,
                                                                               "POWER_SUPPLY_VOLTAGE_NOW=")),
                                                currentNow(getUInt64_tAfterKey(config,
                                                                               "POWER_SUPPLY_CURRENT_NOW=")),
                                                chargeFullDesign(
                                                        getUInt64_tAfterKey(config,
                                                                            "POWER_SUPPLY_CHARGE_FULL_DESIGN=")),
                                                chargeFull(getUInt64_tAfterKey(config,
                                                                               "POWER_SUPPLY_CHARGE_FULL=")),
                                                chargeNow(getUInt64_tAfterKey(config,
                                                                              "POWER_SUPPLY_CHARGE_NOW=")),
                                                capacity(getUInt64_tAfterKey(config,
                                                                             "POWER_SUPPLY_CAPACITY=")) {}

    //returns the power draw in watts
    [[nodiscard]] double getPowerDraw() const {
        return (double) voltageNow * 1e-6 * (double) currentNow * 1e-6;
    }

    //returns the time left in seconds
    [[nodiscard]] uint64_t getTimeLeft() const {
        if (status == full) {
            return 0;
        }

        return (uint64_t) ((double) chargeNow / (double) currentNow * 3600);
    }

private:
    static Status initStatus(std::string_view config) {
        std::string_view str = getConfigElement(config, "POWER_SUPPLY_STATUS");

        if (str == "Discharging") {
            return Status::discharging;
        }

        if (str == "Charging") {
            return Status::charging;
        }

        if (str == "Full") {
            return Status::full;
        }

        return Status::unknown;
    }

    static std::string_view getConfigElement(std::string_view configFile, std::string_view element) {
        std::string::size_type first = configFile.find(element) + element.size() + 1;
        std::string::size_type last = configFile.find('\n', first);

        std::string_view ret{configFile.begin() + first, configFile.begin() + last};

        return ret;
    }
};

#endif //I3GUESSTEMATOR_BATTERY_H
