#include <stdexcept>
#include <ratio>
#include <iostream>

#include <gtest/gtest.h>

#include "etl/math/complex.h"
#include "etl/type_traits.h"

using std::ratio;

namespace etl {
namespace math {

/*
 * Tests for complex numbers.
 *
 * Some of the tests below are disabled on Clang, which has thus far declined
 * to make its builtins for things like `sqrt`, `sin`, and `cos` constexpr.
 */

static_assert(IsComplex<Complex<int>>::value, "");
static_assert(!IsComplex<int>::value, "");
static_assert(IsComplex<Complex<int> const &>::value, "");
static_assert(!IsComplex<int const &>::value, "");
static_assert(!IsComplex<float>::value, "");

static constexpr auto c0 = Complex<float>{};
static_assert(real(c0) == 0, "default complex should have 0 real");
static_assert(imag(c0) == 0, "default complex should have 0 imag");
static_assert(-c0 == c0, "");

static constexpr auto c1 = Complex<float>{1, 0};
static_assert(c1 == 1, "");
static_assert(1 == c1, "");
static_assert(-c1 == Complex<float>{-1, 0}, "");
static_assert(c1 + c1 == Complex<float>{2, 0}, "");
static_assert(c1 - c1 == c0, "");
static_assert(c1 == conjugate(c1), "");
#ifndef __clang__
  static_assert(mag(c1) == 1, "");
#endif

static constexpr auto ci = Complex<float>{0, 1};
static_assert(-ci == conjugate(ci), "");
static_assert(c1 != ci, "");
#ifndef __clang__
  static_assert(mag(ci) == 1, "");
#endif

static constexpr auto int_c0 = Complex<int>{c0};
static_assert(int_c0 == c0, "");

static_assert(c1 + ci == Complex<float>{1, 1}, "");

static_assert(c1 * ci == Complex<float>{0, 1}, "");

template <typename T, typename R, typename I>
struct Properties1 {
  static constexpr auto _r = T{R::num} / T{R::den};
  static constexpr auto _i = T{I::num} / T{I::den};
  static constexpr auto c = complex(_r, _i);

  static_assert(real(c) == _r, "");
  static_assert(imag(c) == _i, "");
  static_assert(sqmag(c) == (_r * _r + _i * _i), "");

  static_assert(imag(conj(c)) == -imag(c), "");

  static_assert(real(c) == (c + conj(c)) / T{2},
      "arithmetic extraction of real component");
  static_assert(real(c) == T{0.5} * (c + conj(c)),
      "arithmetic extraction of real component (multiplication version)");
  static_assert(imag(c) == (c - conj(c)) / complex<T>(0, 2),
      "arithmetic extraction of imaginary coefficient as real");
  static_assert(imag(c) == complex<T>(0, 0.5) * (conj(c) - c),
      "arithmetic extraction of imaginary coefficient "
      "(multiplication version)");
};

static_assert(ci - conj(ci) == complex(0.f, 2.f), "");

template struct Properties1<float, ratio<0>, ratio<0>>;
template struct Properties1<float, ratio<1>, ratio<0>>;
template struct Properties1<float, ratio<0>, ratio<1>>;
template struct Properties1<float, ratio<10>, ratio<1, 2>>;

template <typename T>
std::ostream & operator<<(std::ostream & o, Complex<T> const & c) {
  return o << "complex(" << real(c) << ", " << imag(c) << ")";
}

}  // namespace math
}  // namespace etl
