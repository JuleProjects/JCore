#pragma once

/**
 * @brief   Execution timer
 * @file    exec_timer.hpp
 * @author  Praween Shinonawanik
 */

#include <chrono>
#include <iostream>
#include <string>

namespace jtimer {

using namespace std::chrono;

class ExecTimer {
public:
    //! @brief ExecTimer
    ExecTimer() noexcept : ExecTimer{"", 1} {}

    /**
     * @brief ExecTimer
     * @param[in] name          timer name
     */
    explicit ExecTimer(const char* name) noexcept : ExecTimer{name, 1} {}

    /**
     * @brief ExecTimer
     * @param[in] iteration     the number of iteration
     */
    explicit ExecTimer(size_t iteration) noexcept : ExecTimer{"", iteration} {}

    /**
     * @brief ExecTimer
     * @param[in] name          timer name
     * @param[in] iteration     the number of iteration
     */
    ExecTimer(const char* name, size_t iteration) noexcept
        : name_{name ? name : ""},
          iteration_{iteration},
          t0_{high_resolution_clock::now()} {}

    //! @brief ~ExecTimer
    ~ExecTimer() {
        auto dt = static_cast<double>(Elapsed<std::nano>());
        std::cout << name_ << ": ";
        if (dt > 1e9)
            std::cout << dt * 1e-9 << " s";
        else if (dt > 1e6)
            std::cout << dt * 1e-6 << " ms";
        else if (dt > 1e3)
            std::cout << dt * 1e-3 << " us";
        else
            std::cout << dt << " ns";

        dt /= static_cast<double>(iteration_);
        if (dt > 1e9)
            std::cout << " (" << dt * 1e-9 << " s/cycle)\n";
        else if (dt > 1e6)
            std::cout << " (" << dt * 1e-6 << " ms/cycle)\n";
        else if (dt > 1e3)
            std::cout << " (" << dt * 1e-3 << " us/cycle)\n";
        else
            std::cout << " (" << dt << " ns/cycle)\n";
    }

    //! @brief Update initial time point \a t0
    void Start() { t0_ = high_resolution_clock::now(); }

    //! @return time since the timer was last started
    template <typename Period = std::milli>
    size_t Elapsed() const {
        return duration_cast<duration<size_t, Period>>(high_resolution_clock::now() - t0_).count();
    }

    //! @return time since the timer was last started
    template <typename Period = std::milli>
    size_t Restart() {
        auto time = high_resolution_clock::now();
        auto dt   = duration_cast<duration<size_t, Period>>(time - t0_);
        t0_       = time;
        return dt.count();
    }

private:
    std::string name_;                     //!< name
    std::size_t iteration_;                //!< number of iteration
    time_point<high_resolution_clock> t0_; //!< time point when last started
};

} // namespace jtimer

using JExecTimer = jtimer::ExecTimer;
