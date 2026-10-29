#include "state_b.hpp"

#include <spdlog/spdlog.h>

void StateB::OnEnter(SharedData& sd) {
    spdlog::info("Enter: StateB");
    --sd.x;
}

void StateB::OnExit(SharedData& sd) {
    spdlog::info("Exit : StateB");
    --sd.x;
}

void StateB::Update(SharedData& sd) {
    --sd.x;
    --x;
    --y;
}

void StateB::OnEvent(SharedData&, const EventA& ev) {
    x = ev.x;
    y = ev.x;
}

void StateB::OnEvent(SharedData&, const EventB& ev) {
    x = ev.x;
    y = ev.y;
}
