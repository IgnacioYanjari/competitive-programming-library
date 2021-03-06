//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_MATH_TERNARY_SEARCH_HPP
#define CPL_MATH_TERNARY_SEARCH_HPP

#include <cassert>     // assert
#include <stdexcept>   // runtime_error
#include <type_traits> // is_floating_point

namespace cpl {

/// \brief Finds the maximum of a unimodal function.
///
/// A unimodal function is a function that is either strictly increasing and
/// then strictly decreasing or vice versa (in this case, it must be strictly
/// increasing and then strictly decreasing).
///
/// \param f The input function.
/// \param a The lower bound of the range to search.
/// \param b The upper bound of the range to search.
/// \param tol The maximum absolute error allowed.
/// \param max_iter The maximum number of iterations allowed.
///
/// \pre `a <= b`
/// \pre `tol > 0`
/// \pre `f` must be unimodal in the range `[a, b]` and must contain a maximum
/// (not a minimum).
///
/// \returns The maximum of `f` in the range `[a, b]`.
///
/// \throws std::runtime_error if the maximum number of allowed iterations is
/// exceeded.
///
/// \par Complexity
/// The function `f` will be evaluated twice per iteration. The expected number
/// of iterations is \f$0\f$ if \f$(b - a) \leq (2 \cdot \mathit{tol})\f$,
/// otherwise it is defined by
/// \f[
///    \left\lceil
///      \frac
///      {\log{\left(\frac{b - a}{2 \cdot \mathit{tol}}\right)}}
///      {\log{\left(\frac{3}{2}\right)}}
///   \right\rceil
/// \f]
///
/// \warning The absolute error of the returned result might be greater than
/// `tol` if the function `f` is not precise enough when evaluating numbers
/// close to the maximum. It is more likely to occur when either `a` or
/// `b` is the maximum.
///
template <typename T, typename F>
T ternary_search(const F f, T a, T b, const T tol, const int max_iter) {
  static_assert(std::is_floating_point<T>::value, "");
  // num_iters = ceil(log((b - a) / (2 * tol)) / log(1.5));

  for (int iter = 0; iter < max_iter; ++iter) {
    assert(a <= b);
    if ((b - a) <= 2 * tol)
      return (a + b) / 2;

    const auto left_third = (2 * a + b) / 3;
    const auto right_third = (a + 2 * b) / 3;

    if (f(left_third) < f(right_third))
      a = left_third;
    else
      b = right_third;
  }
  throw std::runtime_error("Max number of iterations exceeded");
}

} // end namespace cpl

#endif // Header guard
