#include "state_a.hpp"

#include <spdlog/spdlog.h>

void StateA::OnEnter(SharedData& sd) {
    spdlog::info("Enter: StateA");
    ++sd.x;
}

void StateA::OnExit(SharedData& sd) {
    spdlog::info("Exit : StateA");
    ++sd.x;
}

void StateA::Update(SharedData& sd) {
    ++sd.x;
    ++x;
    ++y;
}

void StateA::OnEvent(SharedData&, const EventA& ev) {
    x = ev.x;
    y = ev.x;
}

void StateA::OnEvent(SharedData&, const EventB& ev) {
    x = ev.x;
    y = ev.y;
}
