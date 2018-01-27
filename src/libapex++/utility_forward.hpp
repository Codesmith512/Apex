#pragma once

#include "libstl"
#include "type_traits"

/**
 * Defines forward and move functions found in utility
 */

STL_BEGIN

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

STL_END
