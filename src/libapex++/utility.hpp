#pragma once

#include "declval"
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

/* Forward value as lvalue */
template<typename T>
typename enable_if<!is_reference<T>::value, T&>::type
forward(T& t)
{ return t; }

/* Forward lvalue as rvalue */
template<typename T>
typename enable_if<is_rvalue_reference<T>::value, T&&>::type
forward(T& t)
{ return static_cast<T&&>(t); }

/* Forward rvalue as rvalue */
template<typename T>
typename enable_if<is_rvalue_reference<T>::value, T&&>::type
forward(T&& t)
{ return static_cast<T&&>(t); }

/* Forward lvalue as lvalue */
template<typename T>
typename enable_if<is_lvalue_reference<T>::value, T>::type
forward(T t)
{ return t; }

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
  obj = std::forward<U>(new_value);
  return temp;
}

STL_END
