#pragma once

/**
 * @brief   Elapsed timer
 * @file    elapsed_timer.hpp
 * @author  Praween Shinonawanik
 */

#include <chrono>

namespace jcore {

using std::chrono::duration_cast;

template <typename Clock = std::chrono::steady_clock>
struct ElapsedTimer {
  void Start() { t0 = Clock::now(); }

  void Reset() { t0 = Clock::now(); }

  //! @return duration from the last time point \a t0
  template <typename Duration>
  Duration Elapsed() const {
    return duration_cast<Duration>(Clock::now() - t0);
  }

  //! @return duration from the last time point \a t0 and sets t0 to the current
  //! time point
  template <typename Duration>
  Duration ElapsedAndRestart() {
    auto t = Clock::now();
    auto dt = duration_cast<Duration>(t - t0);
    t0 = t;
    return dt;
  }

  //! @brief Returns true if duration has elapsed
  template <typename Rep, typename Period>
  bool HasElapsed(const std::chrono::duration<Rep, Period> &duration) const {
    return Clock::now() - t0 >= duration;
  }

  //! @brief Returns true if duration has elapsed and restart the timer
  template <typename Rep, typename Period>
  bool
  HasElapsedAndRestart(const std::chrono::duration<Rep, Period> &duration) {
    auto t = Clock::now();
    if (t - t0 >= duration) {
      t0 = t;
      return true;
    }
    return false;
  }

  std::chrono::time_point<Clock> t0{Clock::now()};
};

} // namespace jcore
