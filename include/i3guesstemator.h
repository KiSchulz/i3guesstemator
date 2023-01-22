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

#include "writer.h"
#include "i3barWriter.h"
#include "configuration.h"
#include "timeGenerator.h"

class I3Guesstemator {
public:
    std::unique_ptr<Writer> writer;
    int64_t timeToSleep;
    bool work = true;

public:
    I3Guesstemator () : writer(std::make_unique<I3barWriter>()), timeToSleep(1000) {
        writer->pushBack(std::make_unique<TimeGenerator>());
    }

    explicit I3Guesstemator(const Configuration &configuration) {
        //TODO
    }

    void run() {
        while (work) {
            writer->update();
            std::this_thread::sleep_for(std::chrono::seconds(timeToSleep));
        }
    }
};

#endif //I3GUESSTEMATOR_I3GUESSTEMATOR_H
