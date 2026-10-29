#pragma once

#include <jcore/state_machine.hpp>

#include "event.hpp"

class StateA final : public jcore::State<SharedData> {
public:
  void OnEnter(SharedData &sd);

  void OnExit(SharedData &sd);

  void Update(SharedData &sd);

  void OnEvent(SharedData &sd, const EventA &ev);

  void OnEvent(SharedData &sd, const EventB &ev);

  int x{}, y{};
};
