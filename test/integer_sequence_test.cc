#include "etl/integer_sequence.h"

#include <type_traits>

static_assert(etl::IndexSequence<>::size() == 0,
              "empty index sequence should have size zero.");

static_assert(etl::IndexSequence<0, 1, 2>::size() == 3,
              "three-element index sequence should have size three.");

static_assert(etl::Concat<etl::IndexSequence<0, 1, 2>,
                          etl::IndexSequence<4, 5>>::size() == 5, "");

static_assert(etl::MakeIndexSequence<0>::size() == 0, "");
static_assert(etl::MakeIndexSequence<1>::size() == 1, "");
static_assert(etl::MakeIndexSequence<2>::size() == 2, "");

static_assert(std::is_same<etl::MakeIndexSequence<0>,
                           etl::IndexSequence<>>::value, "");
static_assert(std::is_same<etl::MakeIndexSequence<1>,
                           etl::IndexSequence<0>>::value, "");
static_assert(std::is_same<etl::MakeIndexSequence<2>,
                           etl::IndexSequence<0, 1>>::value, "");
static_assert(std::is_same<etl::MakeIndexSequence<3>,
                           etl::IndexSequence<0, 1, 2>>::value, "");
static_assert(std::is_same<etl::MakeIndexSequence<4>,
                           etl::IndexSequence<0, 1, 2, 3>>::value, "");

