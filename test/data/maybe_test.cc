#include <gtest/gtest.h>

#include "etl/data/maybe.h"

using etl::data::Maybe;
using etl::data::nothing;

static_assert(etl::data::IsMaybe<Maybe<int>>::value,
              "Maybe<T> must trigger IsMaybe<T>.");
static_assert(etl::data::IsMaybe<Maybe<int> const>::value,
              "IsMaybe should be insensitive to qualifiers.");
static_assert(etl::data::IsMaybe<Maybe<int> volatile>::value,
              "IsMaybe should be insensitive to qualifiers.");

static_assert(etl::data::IsMaybe<Maybe<int> &>::value,
              "IsMaybe should be insensitive to references.");
static_assert(etl::data::IsMaybe<Maybe<int> &&>::value,
              "IsMaybe should be insensitive to references.");

static_assert(!etl::data::IsMaybe<int>::value,
              "Non-Maybe types should not trigger IsMaybe<T>.");

struct LifeSpy {
  enum class Origin {
    destroyed,
    constructed,
    copy_constructed,
    move_constructed,
    copy_assigned,
    move_assigned,
  };

  static unsigned instances_alive;

  Origin origin;
  unsigned generation;

  LifeSpy() : origin(Origin::constructed), generation(0) {
    ++instances_alive;
  }

  LifeSpy(LifeSpy const &other)
    : origin(Origin::copy_constructed),
      generation(other.generation + 1) {
    ++instances_alive;
  }

  LifeSpy(LifeSpy &&other)
    : origin(Origin::move_constructed),
      generation(other.generation) {
    // instances_alive unchanged.
    other.origin = Origin::destroyed;
  }

  LifeSpy &operator=(LifeSpy const &other) {
    origin = Origin::copy_assigned;
    generation = other.generation + 1;
    // instances_alive unchanged.
    return *this;
  }

  LifeSpy &operator=(LifeSpy && other) {
    origin = Origin::move_assigned;
    generation = other.generation;
    other.origin = Origin::destroyed;
    --instances_alive;
    return *this;
  }

  ~LifeSpy() {
    if (origin != Origin::destroyed) {
      origin = Origin::destroyed;
      --instances_alive;
    }
  }

  static void reset() {
    instances_alive = 0;
  }
};

unsigned LifeSpy::instances_alive = 0;

LifeSpy make_life_spy() {
  return LifeSpy();
}


TEST(Maybe, Basic) {
  Maybe<int> m(nothing);

  ASSERT_FALSE(!!m);

  m = 42;

  ASSERT_TRUE(!!m);
  ASSERT_EQ(42, m.ref());

  m = nothing;

  ASSERT_FALSE(!!m);
}


TEST(Maybe, DefaultCtorNotRequired) {
  class NoDefaultCtor {
   public:
    NoDefaultCtor(int x) : _x(x) {}

   private:
    int _x;
  };

  Maybe<NoDefaultCtor> x(nothing);
  ASSERT_FALSE(!!x);
  x = NoDefaultCtor(12);
  ASSERT_TRUE(!!x);
}

TEST(Maybe, ValueNotConstructedUnlessRequested) {
  class ExplodingDefaultCtor {
   public:
    ExplodingDefaultCtor() { EXPECT_FALSE(true); }
  };

  Maybe<ExplodingDefaultCtor> x(nothing);
  ASSERT_FALSE(!!x);
}

struct CopySpy {
  static unsigned copies;

  CopySpy() = default;
  CopySpy(CopySpy const &) {
    ++copies;
  }
  CopySpy(CopySpy &&) {
    // don't increment copies on move
  }
};

unsigned CopySpy::copies = 0;

TEST(Maybe, CopyConstructEmpty) {
  LifeSpy::reset();

  Maybe<LifeSpy> m(nothing);
  ASSERT_EQ(0, LifeSpy::instances_alive);

  Maybe<LifeSpy> m2(m);
  ASSERT_EQ(0, LifeSpy::instances_alive);
}

TEST(Maybe, CopyConstructFull) {
  LifeSpy::reset();

  LifeSpy original;
  Maybe<LifeSpy> m(original);
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_constructed, m.ref().origin);
  ASSERT_EQ(1, m.ref().generation);

  Maybe<LifeSpy> m2(m);
  ASSERT_EQ(3, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_constructed, m2.ref().origin);
  ASSERT_EQ(2, m2.ref().generation);
}

TEST(Maybe, MoveConstructFull) {
  LifeSpy::reset();

  LifeSpy original;
  Maybe<LifeSpy> m(etl::move(original));
  ASSERT_TRUE(m.is_something());
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::destroyed, original.origin);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);

  Maybe<LifeSpy> m2(etl::move(m));
  ASSERT_TRUE(m.is_nothing());
  ASSERT_TRUE(m2.is_something());
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m2.ref().origin);
  ASSERT_EQ(0, m2.ref().generation);

  // Make sure we're not just totally broken:
  Maybe<LifeSpy> m3(m2);
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_constructed, m3.ref().origin);
  ASSERT_EQ(1, m3.ref().generation);
}

struct IntBox {
  int value;

  explicit IntBox(int x) : value(x) {}
};

TEST(Maybe, CopyConstructConversion) {
  // S -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(m.ref());
  ASSERT_EQ(3, m2.ref().value);
}

TEST(Maybe, CopyConstructConversion2) {
  // Maybe<S> -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(m);
  ASSERT_EQ(3, m2.ref().value);
}

TEST(Maybe, MoveConstructConversion) {
  // S -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(etl::move(m.ref()));
  ASSERT_EQ(3, m2.ref().value);
}

TEST(Maybe, MoveConstructConversion2) {
  // Maybe<S> -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(etl::move(m));
  ASSERT_EQ(3, m2.ref().value);
  ASSERT_FALSE(m.is_something())
    << "Moving a Maybe should empty it.";
}

struct DtorSpy {
  static unsigned alive;

  DtorSpy() { ++alive; }
  DtorSpy(DtorSpy const &) { ++alive; }
  DtorSpy(DtorSpy &&) { }
  ~DtorSpy() { --alive; }
};

unsigned DtorSpy::alive = 0;

TEST(Maybe, NoDestructOnEmpty) {
  LifeSpy::reset();

  {
    Maybe<LifeSpy> m(nothing);
    ASSERT_EQ(0, LifeSpy::instances_alive);
  }
  ASSERT_EQ(0, LifeSpy::instances_alive);
}

TEST(Maybe, DestructOnScopeEnd) {
  LifeSpy::reset();

  LifeSpy d;
  ASSERT_EQ(1, LifeSpy::instances_alive);
  {
    Maybe<LifeSpy> m(d);
    ASSERT_EQ(2, LifeSpy::instances_alive);
  }
  ASSERT_EQ(1, LifeSpy::instances_alive);
}

TEST(Maybe, DestructOnClear) {
  LifeSpy::reset();

  LifeSpy d;
  ASSERT_EQ(1, LifeSpy::instances_alive);

  Maybe<LifeSpy> m(d);
  ASSERT_EQ(2, LifeSpy::instances_alive);

  m.clear();
  ASSERT_TRUE(m.is_nothing());
  ASSERT_EQ(1, LifeSpy::instances_alive);
}

TEST(Maybe, CopyAssignmentIntoEmpty) {
  LifeSpy::reset();

  LifeSpy d;

  Maybe<LifeSpy> m(nothing);
  m = d;
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_constructed, m.ref().origin);
  ASSERT_EQ(1, m.ref().generation);
}

TEST(Maybe, CopyAssignmentIntoFull) {
  LifeSpy::reset();

  LifeSpy d;
  Maybe<LifeSpy> m(d);

  m = d;
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_assigned, m.ref().origin);
  ASSERT_EQ(1, m.ref().generation);
}

TEST(Maybe, MoveAssignmentIntoEmpty) {
  LifeSpy::reset();

  Maybe<LifeSpy> m(nothing);
  m = make_life_spy();
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);
}

TEST(Maybe, MoveAssignmentIntoFull) {
  LifeSpy::reset();

  Maybe<LifeSpy> m(make_life_spy());
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);

  m = make_life_spy();
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_assigned, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);
}

TEST(Maybe, FullComparisons) {
  Maybe<int> two(2);
  Maybe<int> three(3);

  ASSERT_TRUE(two == two);
  ASSERT_FALSE(two != two);

  ASSERT_TRUE(three == three);
  ASSERT_FALSE(three != three);

  ASSERT_FALSE(two == three);
  ASSERT_FALSE(three == two);

  ASSERT_TRUE(two != three);
  ASSERT_TRUE(three != two);
}

TEST(Maybe, EmptyComparisons) {
  Maybe<int> empty1(nothing);
  Maybe<int> empty2(nothing);
  Maybe<int> full(3);

  ASSERT_TRUE(empty1 == empty1);
  ASSERT_FALSE(empty1 != empty1);

  ASSERT_TRUE(empty1 == empty2);
  ASSERT_FALSE(empty1 != empty2);

  ASSERT_FALSE(full == empty1);
  ASSERT_TRUE(full != empty1);

  ASSERT_FALSE(empty1 == full);
  ASSERT_TRUE(empty1 != full);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
