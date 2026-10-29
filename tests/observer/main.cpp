#include <gtest/gtest.h>

#include <random>

#include <jcore/observer.hpp>

static std::random_device rd;
static std::mt19937_64 gen{rd()};

struct ObsInt : jcore::Observer<int, int &> {
  ObsInt() { on_notify.Bind<&ObsInt::OnNotify>(this); }

  void OnNotify(int x_, int &y_) {
    x = x_;
    y_ += x;
  }

  int x{};
};

TEST(Observer, Int) {
  std::uniform_int_distribution dis{std::numeric_limits<int>::lowest()};

  std::array<ObsInt, 3> observers;
  jcore::Observer<int, int &>::Subject subj;

  for (auto &obs : observers) { subj.Add(obs); }

  const int x_ref = dis(gen);
  int y_ref{}, y{};

  subj.Notify(x_ref, y);

  for (const auto &obs : observers) {
    y_ref += x_ref;
    ASSERT_EQ(x_ref, obs.x);
  }

  ASSERT_EQ(y_ref, y);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
