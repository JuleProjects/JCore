#include <jcore/ev_queue.hpp>

#include <random>

#include <gtest/gtest.h>

#include "state_a.hpp"
#include "state_b.hpp"

static std::random_device rd;
static std::mt19937_64 gen;

TEST(JCore, StateMachine) {
  std::uniform_int_distribution dis{std::numeric_limits<int>::lowest()};

  SharedData shared_data;
  jcore::StateMachine<SharedData, StateA, StateB> sm{shared_data};
  // jcore::EvQueue<EventA, EventB> ev_q{16};
  jcore::TsEvQueue<EventA, EventB> ev_q{16};

  int x_ref = 0;
  int a     = dis(gen);
  int b     = dis(gen);

  // StateA
  // ----------------------------------------------------------------------------------------------------------
  sm.RequestTransition<StateA>();
  sm.Update(ev_q); // Update current state and transition to StateA
  ASSERT_EQ(shared_data.x, x_ref);

  sm.Update(ev_q); // StateA::Update()
  ASSERT_EQ(shared_data.x, ++x_ref);

  ev_q.Push(EventA{a});
  sm.Update(ev_q);

  if (auto v = std::get_if<StateA>(&sm.state)) {
    ASSERT_EQ(shared_data.x, ++x_ref);
    ASSERT_EQ(v->x, ++a);
    ASSERT_EQ(v->y, a);
  } else {
    ASSERT_TRUE(0);
  }

  ev_q.Push(EventB{a, b});
  sm.Update(ev_q);

  if (auto v = std::get_if<StateA>(&sm.state)) {
    ASSERT_EQ(shared_data.x, ++x_ref);
    ASSERT_EQ(v->x, ++a);
    ASSERT_EQ(v->y, ++b);
  } else {
    ASSERT_TRUE(0);
  }
  // StateA
  // ----------------------------------------------------------------------------------------------------------

  // StateB
  // ----------------------------------------------------------------------------------------------------------
  sm.RequestTransition<StateB>();
  sm.Update(ev_q); // Update current state and transition to StateB
  ASSERT_EQ(shared_data.x, ++x_ref);

  sm.Update(ev_q); // StateB::Update()
  ASSERT_EQ(shared_data.x, --x_ref);

  ev_q.Push(EventA{a});
  sm.Update(ev_q);

  if (auto v = std::get_if<StateB>(&sm.state)) {
    ASSERT_EQ(shared_data.x, --x_ref);
    ASSERT_EQ(v->x, --a);
    ASSERT_EQ(v->y, a);
  } else {
    ASSERT_TRUE(0);
  }

  ev_q.Push(EventB{a, b});
  sm.Update(ev_q);

  if (auto v = std::get_if<StateB>(&sm.state)) {
    ASSERT_EQ(shared_data.x, --x_ref);
    ASSERT_EQ(v->x, --a);
    ASSERT_EQ(v->y, --b);
  } else {
    ASSERT_TRUE(0);
  }
  // StateB
  // ----------------------------------------------------------------------------------------------------------

  sm.Stop();
  sm.Update(ev_q);
  ASSERT_EQ(shared_data.x, --x_ref);
  ASSERT_EQ(sm.Index(), 0);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
