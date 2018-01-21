#pragma once

#include "libstl"
#include "type_traits"

/**
 * The definition of the well-documented <utility> file
 *
 * http://en.cppreference.com/w/cpp/header/utility
 */

STL_BEGIN

/**
 * Swaps the values of two objects
 */
template<typename T>
void swap(T& a, T& b)
{
  T c = a;
  a = b;
  b = c;
}

/**
 * Used to ensure that a reference type is preserved
 */
namespace detail
{
  template<typename T>
  struct identity
  { using type = T; };
};

template<typename T>
T&& forward(typename remove_reference<T>::type& ref)
{ return static_cast<typename detail::identity<T>::type&&>(ref); }

template<typename T>
T&& forward(typename remove_reference<T>::type&& ref)
{ return static_cast<typename detail::identity<T>::type&&>(ref); }

/**
 * Creates an xvalue reference
 */
template<typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& t)
{ return static_cast<typename std::remove_reference<T>::type&&>(t); }

/**
 * Replaces the argument with a new value, returning it's previous value
 */
template<typename T, typename U = T>
T exchange(T& obj, U&& new_value)
{
  T temp = std::move(obj);
  obj = std::forward<U&&>(new_value);
  return temp;
}

STL_END
