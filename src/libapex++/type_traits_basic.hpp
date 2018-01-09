#pragma once

#include "libstl"

STL_BEGIN

/**
 * Defines a compile-time integral constant
 */
template<typename T, T v>
struct integral_constant
{
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant;
  constexpr operator value_type() const { return value; }
  constexpr value_type operator()() const { return value; }
};

/**
 * Helper template for boolean integral_constant
 */
template<bool B>
using bool_constant = integral_constant<bool, B>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

STL_END
