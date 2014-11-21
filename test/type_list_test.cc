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

// Searching
static_assert(
    std::is_same<char *,
                 TypeList<int, char *, bool, void *>::FindFirst<std::is_pointer>
                 >::value, "");
