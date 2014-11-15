#include "etl/non_null.h"

#include <memory>

using etl::NonNull;
using etl::null_check;

// A way of declaring properties as applying "forall T".
template <typename P>
struct GenericProperties {
  static_assert(std::is_pointer<P>::value == true,
                "GenericProperties must be used with a pointer type.");

  static_assert(std::is_pointer<NonNull<P>>::value == false,
                "std::is_pointer must distinguish NonNull from raw pointers.");
};

// Check the properties above for a variety of types:
template struct GenericProperties<void *>;  // This old weirdo
template struct GenericProperties<char *>;  // A non-const pointer
template struct GenericProperties<char const *>;  // A const pointer
template struct GenericProperties<int volatile *>;  // A volatile pointer


constexpr NonNull<char const *> hello { "hello" };

static_assert(static_cast<char const *>(hello),
              "this pointer should be non-null");

constexpr auto hello_copy = hello;
static_assert(hello == hello_copy, "pointers should be equal");

struct HasNonNull {
  NonNull<char const *> member;
};

constexpr HasNonNull a_struct { null_check("hello") };
constexpr NonNull<char const *> member = a_struct.member;

// Three different ways of getting at the raw pointer.
constexpr char const * pointer = member.get();
constexpr char const * pointer2 = static_cast<char const *>(member);
constexpr char const * pointer3(member);

// Should not compile:
// constexpr NonNull<char const *> null { nullptr };
// constexpr NonNull<char const *> zero { 0 };
// constexpr NonNull<char const *> zero2 { NULL };

// Should not compile:
// static_assert(hello == nullptr, "hi");
// static_assert(hello == 0, "hi");
