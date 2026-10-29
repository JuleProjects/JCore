#include <benchmark/benchmark.h>

#include <random>

#include <spdlog/spdlog.h>

#include <jcore/ev_queue.hpp>

#ifdef WITH_ENTT
#include <entt/signal/dispatcher.hpp>
#endif

#define QSIZE 32

static std::random_device rd;
static std::mt19937_64 gen;
static std::uniform_int_distribution dis{std::numeric_limits<int>::lowest()};

struct EventA {
  int x;
};

struct EventB {
  int y, z;
};

class Handler {
public:
  void OnEvent(EventA const &ev) { x_ = ev.x; }

  void OnEvent(EventB const &ev) {
    y_ = ev.y;
    z_ = ev.z;
  }

private:
  int x_, y_, z_;
};

static void BM_Push_EvQueue(benchmark::State &state) {
  jcore::EvQueue<EventA, EventB> ev_q{QSIZE};
  const int a = dis(gen), b = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      ev_q.Push(EventA{a});
      ev_q.Push(EventB{a, b});
    }
    ev_q.Clear();
  }
}
BENCHMARK(BM_Push_EvQueue);

#ifdef WITH_JCORE_TS_EVENT_QUEUE
static void BM_Push_TsEvQueue(benchmark::State &state) {
  jcore::TsEvQueue<EventA, EventB> ev_q{QSIZE};
  const int a = dis(gen), b = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      ev_q.Push(EventA{a});
      ev_q.Push(EventB{a, b});
    }
    ev_q.Clear();
  }
}
BENCHMARK(BM_Push_TsEvQueue);
#endif // WITH_JCORE_TS_EVENT_QUEUE

#ifdef WITH_ENTT
static void BM_Push_EnTT(benchmark::State &state) {
  entt::dispatcher dispatcher;
  const int a = dis(gen), b = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      dispatcher.enqueue(EventA{a});
      dispatcher.enqueue(EventB{a, b});
    }
    dispatcher.clear();
  }
}
BENCHMARK(BM_Push_EnTT);
#endif // WITH_ENTT

static void BM_Dispatch_EvQueue(benchmark::State &state) {
  jcore::EvQueue<EventA, EventB> ev_q{QSIZE};
  const int a = dis(gen), b = dis(gen);
  Handler handler;

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      ev_q.Push(EventA{a});
      ev_q.Push(EventB{a, b});
    }
    ev_q.Dispatch(handler);
  }
}
BENCHMARK(BM_Dispatch_EvQueue);

#ifdef WITH_JCORE_TS_EVENT_QUEUE
static void BM_Dispatch_TsEvQueue(benchmark::State &state) {
  jcore::TsEvQueue<EventA, EventB> ev_q{QSIZE};
  const int a = dis(gen), b = dis(gen);
  Handler handler;

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      ev_q.Push(EventA{a});
      ev_q.Push(EventB{a, b});
    }
    ev_q.Dispatch(handler);
  }
}
BENCHMARK(BM_Dispatch_TsEvQueue);
#endif // WITH_JCORE_TS_EVENT_QUEUE

#ifdef WITH_ENTT
static void BM_Dispatch_EnTT(benchmark::State &state) {
  entt::dispatcher dispatcher;
  const int a = dis(gen), b = dis(gen);
  Handler handler;
  dispatcher.sink<EventA>()
      .connect<static_cast<void (Handler::*)(EventA const &)>(
          &Handler::OnEvent)>(handler);
  dispatcher.sink<EventB>()
      .connect<static_cast<void (Handler::*)(EventB const &)>(
          &Handler::OnEvent)>(handler);

  for ([[maybe_unused]] auto _ : state) {
    for (std::size_t i = 0; i < QSIZE / 2; ++i) {
      dispatcher.enqueue(EventA{a});
      dispatcher.enqueue(EventB{a, b});
    }
    dispatcher.update();
  }
}
BENCHMARK(BM_Dispatch_EnTT);
#endif // WITH_ENTT

BENCHMARK_MAIN();
