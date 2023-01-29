/*
 * Copyright (c) 12.12.22 15:31 Kilian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_UPDATINGAVERAGE_H
#define I3GUESSTEMATOR_UPDATINGAVERAGE_H

#include <vector>

template<typename T>
class UpdatingAverage {
    std::vector<T> entries;
    std::size_t maxSize, idx;
    T sum;
public:
    explicit UpdatingAverage(std::size_t size) : maxSize(size), idx(0), sum(0) {}

    void push(T el) {
        if (entries.size() < maxSize) {
            entries.push_back(el);
        } else {
            sum -= entries[idx];
            entries[idx] = el;
        }
        sum += entries[idx];

        idx = (idx + 1) % entries.size();
    }

    T getAverage() {
        return sum / entries.size();
    }

    [[nodiscard]] std::size_t getNumEntries() const {
        return entries.size();
    }
};

#endif //I3GUESSTEMATOR_UPDATINGAVERAGE_H
