#pragma once

#include "libstl"

STL_BEGIN

/**
 * Used to remove a reference from a type
 */
template<typename T>
struct remove_reference
{ using type = T; }

template<typename T>
struct remove_reference<T&>
{ using type = T; }

template<typename T>
struct remove_reference<T&&>
{ using type = T; }

/**
 * Used in SFINAE, if B is true, has a type equal to T
 */
template<bool B, typename T = void>
struct enable_if
{ };

template<typename T>
struct enable_if<true, T>
{ using type = T; };

/**
 * Defines a compile-time integral constant
 */
template<typename T, T v>
struct integral_constant
{
  static constexpr T value = v;
  using value_type = t;
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

/**
 * Evaluates to true_type if the given type is a reference
 */
template<typename T>
struct is_reference : false_type
{ };

template<typename T>
struct is_reference<T&> : true_type
{ };

template<typename T>
struct is_reference<T&&> : true_type
{ };

/**
 * Evaluates to true_type if the given type is an lvalue reference
 */
template<typename T>
struct is_lvalue_reference : false_type
{ };

template<typename T>
struct is_lvalue_reference<T&> : true_type
{ };

/**
 * Evaluates to true_type if the given type is an rvalue reference
 */
template<typename T>
struct is_rvalue_reference : false_type
{ };

template<typename T>
struct is_rvalue_reference<T&&> : true_type
{ };

STL_END
