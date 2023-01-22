/*
 * Copyright (c) 21.01.23 18:57 Kilian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_CONFIGURATION_H
#define I3GUESSTEMATOR_CONFIGURATION_H

#include <fstream>
#include <filesystem>
#include <unordered_map>

class Configuration {
    std::unordered_map<std::string, Configuration> children;
    std::unordered_map<std::string, std::string> values;

public:
    explicit Configuration(std::string_view content) {
        //TODO parse content
    }

    explicit Configuration(const std::filesystem::path &path) {
        //TODO
    }

    Configuration getChild(const std::string &name) {
        return children.at(name);
    }

    std::string_view getValue(const std::string& name) {
        return values.at(name);
    }
};

#endif //I3GUESSTEMATOR_CONFIGURATION_H
