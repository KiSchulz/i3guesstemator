/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#include "i3guesstemator.h"

#include <memory>

int main(int argc, char *argv[]) {
    //TODO: add a reader for inputs from the bar

    std::unique_ptr<I3Guesstemator> i3Guesstemator;

    if (argc == 2) {
        Configuration configuration{std::filesystem::path{argv[1]}};
        i3Guesstemator = std::make_unique<I3Guesstemator>(configuration);
    } else {
        i3Guesstemator = std::make_unique<I3Guesstemator>();
    }

    i3Guesstemator->run();

    return 0;
}
