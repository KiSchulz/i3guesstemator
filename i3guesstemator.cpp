/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#include <memory>

#include "yaml-cpp/yaml.h"
#include "i3guesstemator.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<I3Guesstemator> i3Guesstemator;

    if (argc == 2) {
        YAML::Node config;

        try {
            config = YAML::LoadFile(std::filesystem::path(argv[1]));
        } catch (const std::runtime_error &error) {
            std::cerr << error.what() << "\n";
            return -1;
        }

        i3Guesstemator = std::make_unique<I3Guesstemator>(config);
    } else {
        i3Guesstemator = std::make_unique<I3Guesstemator>();
    }

    try {
        i3Guesstemator->run();
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << "\n";
    }

    return 0;
}
