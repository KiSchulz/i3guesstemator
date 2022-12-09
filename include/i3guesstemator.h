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

class I3Guesstemator {
public:
    std::shared_ptr<Writer> writer;
    int64_t timeToSleep;
    bool work = true;
public:
    I3Guesstemator(std::shared_ptr<Writer> writer, int64_t timeToSleep) : writer(std::move(writer)),
                                                                          timeToSleep(timeToSleep) {

    }

    void run() {
        while (work) {
            writer->update();
            std::this_thread::sleep_for(std::chrono::seconds(timeToSleep));
        }
    }
};

#endif //I3GUESSTEMATOR_I3GUESSTEMATOR_H
