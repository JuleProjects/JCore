#include <gtest/gtest.h>

#include <random>

#include <jcore/delegate.hpp>

static std::random_device rd;
static std::mt19937_64 gen{rd()};
static std::uniform_int_distribution dis{std::numeric_limits<int>::lowest()};

static int FreeFunction(int x, int y) { return x + y; }

struct Obj {
  int Function(int x, int y) { return a_ = x + y; }
  int ConstFunction(int x, int y) const { return x + y; }

  int a_{};
};

template <typename T>
static void Check(T &del) {
  const int x = dis(gen), y = dis(gen);

  ASSERT_TRUE(del);
  ASSERT_EQ(del(x, y), x + y);

  del.Unbind();
  ASSERT_FALSE(del);
}

TEST(Delegate, FreeFunction) {
  jcore::Delegate<int(int, int)> del;
  del.Bind<FreeFunction>();

  Check(del);
}

TEST(Delegate, Object) {
  jcore::Delegate<int(int, int)> del;
  Obj obj;

  del.Bind<&Obj::Function>(&obj);
  Check(del);

  del.Bind<&Obj::ConstFunction>(&obj);
  Check(del);
}

TEST(Delegate, ConstObject) {
  jcore::Delegate<int(int, int)> del;
  const Obj obj;

  del.Bind<&Obj::ConstFunction>(&obj);
  Check(del);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
