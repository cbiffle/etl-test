#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "etl/type_list.h"

using etl::TypeList;

/*
 * There's not a lot one can do with an empty TypeList.
 */

static_assert(TypeList<>::size() == 0, "Empty TypeList must have size 0");

template <typename T> struct BogusFn;

static_assert(std::is_same<TypeList<>,
                           TypeList<>::Map<BogusFn>>::value,
              "TypeList<>::Map<F> must be TypeList<>, independent of F");

/*
 * Non-empty TypeLists are a bit more interesting.
 */

struct A {};
struct B {};
struct C {};

// Repetition
static_assert(std::is_same<etl::Repeat<A, 3>, TypeList<A, A, A>>::value, "");

// Sizing
static_assert(TypeList<A, B, C>::size() == 3, "");

// Indexing
static_assert(std::is_same<A, TypeList<A, B, C>::At<0>>::value, "");
static_assert(std::is_same<B, TypeList<A, B, C>::At<1>>::value, "");
static_assert(std::is_same<C, TypeList<A, B, C>::At<2>>::value, "");

// Mapping
static_assert(
    std::is_same<TypeList<unsigned char, unsigned int>,
                 TypeList<signed char, signed int>::Map<std::make_unsigned>
                 >::value, "");

// Searching by predicate, returning the type.
static_assert(
    std::is_same<char *,
                 TypeList<int, char *, bool, void *>::FindFirst<std::is_pointer>
                 >::value, "");

// Searching by identity, returning the index.
static_assert(TypeList<int, bool, char>::index_of<int>() == 0, "");
static_assert(TypeList<int, bool, char>::index_of<bool>() == 1, "");
static_assert(TypeList<int, bool, char>::index_of<char>() == 2, "");

// Checking uniqueness.
static_assert(TypeList<>::all_unique, "");
static_assert(TypeList<int>::all_unique, "");
static_assert(TypeList<int, bool, char>::all_unique, "");
static_assert(TypeList<int, bool, int>::all_unique == false, "");

// Checking containment.
static_assert(TypeList<>::contains<int>() == false, "");
static_assert(TypeList<char>::contains<int>() == false, "");
static_assert(TypeList<int>::contains<int>(), "");
static_assert(TypeList<char, bool>::contains<int>() == false, "");
static_assert(TypeList<char, int>::contains<int>(), "");

// Deriving aggregate properties.
static_assert(etl::MaxSizeOf<TypeList<int>>::value == sizeof(int), "");
static_assert(etl::MaxAlignOf<TypeList<int>>::value == alignof(int), "");
