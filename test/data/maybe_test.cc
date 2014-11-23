#include <stdexcept>

#include <gtest/gtest.h>

#include "etl/data/maybe.h"

using etl::data::Maybe;
using etl::data::nothing;

/*******************************************************************************
 * IsMaybe<T>
 */

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


/*******************************************************************************
 * Constexpr Maybe<T>.  It's pretty limited, but it works for certain things.
 */

constexpr Maybe<int> empty_maybe(nothing);
static_assert(!empty_maybe, "");

constexpr Maybe<int> full_maybe(3);
static_assert(full_maybe, "");
static_assert(full_maybe.const_ref() == 3, "");


/*******************************************************************************
 * Size and alignment
 */

static_assert(alignof(bool) == 1 && sizeof(bool) == 1,
              "These tests assume bool acts like char.");

static_assert(sizeof(Maybe<bool>) == 2, "");
static_assert(alignof(Maybe<bool>) == 1, "");


/*******************************************************************************
 * Dynamic Maybe<T> tests for RAII and whatnot.
 */

struct LifeSpy {
  enum class Origin {
    destroyed,
    constructed,
    copy_constructed,
    move_constructed,
    copy_assigned,
    move_assigned,
    moved_from,
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
    ++instances_alive;
    other.origin = Origin::moved_from;
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
    other.origin = Origin::moved_from;
    // instances_alive unchanged.
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

static LifeSpy make_life_spy() {
  return LifeSpy();
}

class MaybeTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    LifeSpy::reset();
  }
};


TEST_F(MaybeTest, Basic) {
  Maybe<int> m(nothing);

  ASSERT_FALSE(!!m);

  m = 42;

  ASSERT_TRUE(!!m);
  ASSERT_EQ(42, m.ref());

  m = nothing;

  ASSERT_FALSE(!!m);
}


TEST_F(MaybeTest, DefaultCtorNotRequired) {
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

TEST_F(MaybeTest, ValueNotConstructedUnlessRequested) {
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

TEST_F(MaybeTest, CopyConstructEmpty) {
  Maybe<LifeSpy> m(nothing);
  ASSERT_EQ(0, LifeSpy::instances_alive);

  Maybe<LifeSpy> m2(m);
  ASSERT_EQ(0, LifeSpy::instances_alive);
}

TEST_F(MaybeTest, CopyConstructFull) {
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

TEST_F(MaybeTest, MoveConstructFromObject) {
  LifeSpy original;
  Maybe<LifeSpy> m(etl::move(original));
  ASSERT_TRUE(m.is_something());
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::moved_from, original.origin);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);
}

TEST_F(MaybeTest, MoveFrom) {
  Maybe<LifeSpy> origin(etl::data::in_place);

  Maybe<LifeSpy> dest(etl::move(origin));
  ASSERT_TRUE(origin.is_something())
    << "A moved-from Maybe should not clear itself";
  ASSERT_TRUE(dest.is_something())
    << "A moved-to Maybe should be full iff the origin was";
  ASSERT_EQ(2, LifeSpy::instances_alive)
    << "A moved-from Maybe should not destruct its contents, but rather "
       "allow the type's internal 'zombie' handling to take over";
  ASSERT_EQ(LifeSpy::Origin::moved_from, origin.ref().origin);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, dest.ref().origin);
  ASSERT_EQ(origin.ref().generation, dest.ref().generation);
}

struct IntBox {
  int value;

  explicit IntBox(int x) : value(x) {}
};

TEST_F(MaybeTest, CopyConstructConversion) {
  // S -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(etl::data::in_place, m.ref());
  ASSERT_EQ(3, m2.ref().value);
}

TEST_F(MaybeTest, CopyConstructConversion2) {
  // Maybe<S> -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(m);
  ASSERT_EQ(3, m2.ref().value);
}

TEST_F(MaybeTest, MoveConstructConversion) {
  // S -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(etl::data::in_place, etl::move(m.ref()));
  ASSERT_EQ(3, m2.ref().value);
}

TEST_F(MaybeTest, MoveConstructConversion2) {
  // Maybe<S> -> Maybe<T> when T is constructible from S.
  Maybe<int> m(3);

  Maybe<IntBox> m2(etl::move(m));
  ASSERT_EQ(3, m2.ref().value);
  ASSERT_TRUE(m.is_something())
    << "A moved-from maybe should not clear itself";
}

struct DtorSpy {
  static unsigned alive;

  DtorSpy() { ++alive; }
  DtorSpy(DtorSpy const &) { ++alive; }
  DtorSpy(DtorSpy &&) { }
  ~DtorSpy() { --alive; }
};

unsigned DtorSpy::alive = 0;

TEST_F(MaybeTest, NoDestructOnEmpty) {
  {
    Maybe<LifeSpy> m(nothing);
    ASSERT_EQ(0, LifeSpy::instances_alive);
  }
  ASSERT_EQ(0, LifeSpy::instances_alive);
}

TEST_F(MaybeTest, DestructOnScopeEnd) {
  LifeSpy d;
  ASSERT_EQ(1, LifeSpy::instances_alive);
  {
    Maybe<LifeSpy> m(d);
    ASSERT_EQ(2, LifeSpy::instances_alive);
  }
  ASSERT_EQ(1, LifeSpy::instances_alive);
}

TEST_F(MaybeTest, DestructOnClear) {
  LifeSpy d;
  ASSERT_EQ(1, LifeSpy::instances_alive);

  Maybe<LifeSpy> m(d);
  ASSERT_EQ(2, LifeSpy::instances_alive);

  m.clear();
  ASSERT_TRUE(m.is_nothing());
  ASSERT_EQ(1, LifeSpy::instances_alive);
}

TEST_F(MaybeTest, CopyAssignmentIntoEmpty) {
  LifeSpy d;

  Maybe<LifeSpy> m(nothing);
  m = d;
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_constructed, m.ref().origin);
  ASSERT_EQ(1, m.ref().generation);
}

TEST_F(MaybeTest, CopyAssignmentIntoFull) {
  LifeSpy d;
  Maybe<LifeSpy> m(d);

  m = d;
  ASSERT_EQ(2, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::copy_assigned, m.ref().origin);
  ASSERT_EQ(1, m.ref().generation);
}

TEST_F(MaybeTest, MoveAssignmentIntoEmpty) {
  Maybe<LifeSpy> m(nothing);
  m = make_life_spy();
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);
}

TEST_F(MaybeTest, MoveAssignmentIntoFull) {
  Maybe<LifeSpy> m(make_life_spy());
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_constructed, m.ref().origin);

  m = make_life_spy();
  ASSERT_EQ(1, LifeSpy::instances_alive);
  ASSERT_EQ(LifeSpy::Origin::move_assigned, m.ref().origin);
  ASSERT_EQ(0, m.ref().generation);
}

TEST_F(MaybeTest, FullComparisons) {
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

TEST_F(MaybeTest, EmptyComparisons) {
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

  ASSERT_TRUE(empty1 == nothing);
  ASSERT_TRUE(nothing == empty1);
}

static Maybe<int> make_int(bool f) {
  if (f) {
    return Maybe<int>(3);
  } else {
    return nothing;
  }
}

TEST_F(MaybeTest, ReturnFromFn) {
  Maybe<int> x = make_int(true);
  ASSERT_EQ(3, x.ref());
}


/*******************************************************************************
 * Policy Tests
 */

template <typename T>
using MaybeX = Maybe<T, etl::data::AssertMaybeCheckPolicy>;

TEST_F(MaybeTest, AssertBasic) {
  MaybeX<int> x(3);
  ASSERT_EQ(3, x.ref());
}

TEST_F(MaybeTest, AssertIncorrect) {
  MaybeX<int> x(nothing);
  ASSERT_THROW(x.ref(), std::logic_error);
}
