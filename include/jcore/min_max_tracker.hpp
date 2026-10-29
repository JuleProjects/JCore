#pragma once

/**
 * @brief   Tracks the minimum and maximum values from a sequence of inputs.
 * @file    MinMaxTracker.hpp
 * @author  Praween Shinonawanik
 */

#include "common/concepts.hpp"

#include <algorithm>

namespace jcore {

template <Arithmetic T>
class MinMaxTracker {
public:
    void Update(T val) {
        min_ = std::min(min_, val);
        max_ = std::max(max_, val);
    }

    void Reset() {
        min_ = std::numeric_limits<T>::max();
        max_ = std::numeric_limits<T>::lowest();
    }

    T min() const { return min_; }
    T max() const { return max_; }

private:
    T min_ = std::numeric_limits<T>::max();
    T max_ = std::numeric_limits<T>::lowest();
};

} // namespace jcore
