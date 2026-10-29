#include <benchmark/benchmark.h>

#include <random>

#include <jcore/delegate.hpp>

#ifdef WITH_ENTT
#include <entt/signal/delegate.hpp>
#endif

static int FreeFunction(int i, int j) { return i + j; }

std::random_device rd;
std::mt19937_64 gen{rd()};
std::uniform_int_distribution dis{std::numeric_limits<int>::lowest()};

struct Obj {
  int Function(int i, int j) { return a_ = i + j; }
  int ConstFunction(int i, int j) const { return i + j; }
  int a_;
};

/* Free function -------------------------------------------------------------*/
static void FreeFunction_Std(benchmark::State &state) {
  std::function f{FreeFunction};
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(FreeFunction_Std);

static void FreeFunction_Delegate(benchmark::State &state) {
  jcore::Delegate<int(int, int)> f;
  f.Bind<FreeFunction>();
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(FreeFunction_Delegate);

#ifdef WITH_ENTT
static void FreeFunction_EnTT(benchmark::State &state) {
  entt::delegate<int(int, int)> f;
  f.connect<FreeFunction>();
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(FreeFunction_EnTT);
#endif // WITH_ENTT
/* Free function -------------------------------------------------------------*/

/* Member function -----------------------------------------------------------*/
static void MeberFunction_Std(benchmark::State &state) {
  using namespace std::placeholders;
  Obj obj;
  std::function<int(int, int)> f = std::bind(&Obj::Function, &obj, _1, _2);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(MeberFunction_Std);

static void MeberFunction_Delegate(benchmark::State &state) {
  jcore::Delegate<int(int, int)> f;
  Obj obj;
  f.Bind<&Obj::Function>(&obj);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(MeberFunction_Delegate);

#ifdef WITH_ENTT
static void MeberFunction_EnTT(benchmark::State &state) {
  entt::delegate<int(int, int)> f;
  Obj obj;
  f.connect<&Obj::Function>(&obj);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(MeberFunction_EnTT);
#endif // WITH_ENTT
/* Member function -----------------------------------------------------------*/

/* Constant Member function --------------------------------------------------*/
static void ConstMeberFunction_Std(benchmark::State &state) {
  using namespace std::placeholders;
  Obj obj;
  std::function<int(int, int)> f = std::bind(&Obj::ConstFunction, &obj, _1, _2);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(ConstMeberFunction_Std);

static void ConstMeberFunction_Delegate(benchmark::State &state) {
  jcore::Delegate<int(int, int)> f;
  Obj obj;
  f.Bind<&Obj::ConstFunction>(&obj);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(ConstMeberFunction_Delegate);

#ifdef WITH_ENTT
static void ConstMeberFunction_EnTT(benchmark::State &state) {
  entt::delegate<int(int, int)> f;
  Obj obj;
  f.connect<&Obj::ConstFunction>(&obj);
  const int x = dis(gen), y = dis(gen);

  for ([[maybe_unused]] auto _ : state) {
    auto out = f(x, y);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(ConstMeberFunction_EnTT);
#endif // WITH_ENTT
/* Constant Member function --------------------------------------------------*/

BENCHMARK_MAIN();
