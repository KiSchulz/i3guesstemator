/*
 * Copyright (c) 12.12.22 15:31 Kiian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_UPDATINGAVERAGE_H
#define I3GUESSTEMATOR_UPDATINGAVERAGE_H

#include <array>

template<typename T, std::size_t size>
class UpdatingAverage {
    std::array<T, size> entries;
    std::size_t idx, numElements;
    T sum;
public:
    UpdatingAverage() : idx(0), numElements(0), sum(0) {}

    void push(T el) {
        if (numElements < entries.size()) {
            numElements++;
        }

        sum -= entries[idx];
        entries[idx] = el;
        sum += entries[idx];

        idx = (idx + 1) % entries.size();
    }

    T getAverage() {
        return sum / numElements;
    }
};

#endif //I3GUESSTEMATOR_UPDATINGAVERAGE_H
