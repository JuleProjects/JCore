#pragma once

/**
 * @brief   State Machine
 * @file    StateMachine.hpp
 * @author  Praween Shinonawanik
 */

#include <functional>
#include <variant>

namespace jcore {

//! State interface
template <typename SharedData>
struct State {
    void Enter(SharedData&) {};

    void Update(SharedData&) {}

    void Exit(SharedData&) {}

    void OnEvent(SharedData&, const auto&) {}
};

//! State machine
template <typename SharedData, typename... States>
class StateMachine {
    using StateStop    = State<SharedData>;
    using StateVariant = std::variant<StateStop, States...>;

public:
    explicit StateMachine(SharedData& shared_data) noexcept : shared_data_{shared_data} {}

    //! @brief Updates the current state and then changes state if needed
    void Update() {
        std::visit([this](auto& s) { s.Update(shared_data_.get()); }, state);
        TransitionIfNeeded();
    }

    //! @brief Dispatchs all events, Updates the current state, and then changes state if needed
    template <typename EventQueue>
    void Update(EventQueue& ev_q) {
        std::visit(
            [&](auto& state) {
                ev_q.Dispatch(state, shared_data_.get());
                state.Update(shared_data_.get());
            },
            state);
        TransitionIfNeeded();
    }

    //! @brief Transition to next state /a State
    template <typename State, typename... Args>
    void RequestTransition(Args&&... args) {
        pending_transition_ = [this, ... args = std::forward<Args>(args)]() { TransitionTo<State>(args...); };
    }

    //! @brief Transition to stop state
    void Stop() { RequestTransition<StateStop>(); }

    constexpr std::size_t Index() const noexcept { return state.index(); }

    StateVariant state;

private:
    //! @brief Transition to next state /a State
    template <typename State, typename... Args>
    void TransitionTo(Args&&... args) {
        std::visit([this](auto&& state) { state.Exit(shared_data_.get()); }, state);
        state.template emplace<State>(std::forward<Args>(args)...);
        std::visit([this](auto&& state) { state.Enter(shared_data_.get()); }, state);
    }

    void TransitionIfNeeded() {
        if (!pending_transition_) { return; }
        std::invoke(pending_transition_);
        pending_transition_ = nullptr;
    }

    std::reference_wrapper<SharedData> shared_data_;
    std::function<void(void)> pending_transition_;
};

} // namespace jcore
