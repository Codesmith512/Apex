#pragma once

#include "utility_forward"
#include "libstl"
#include "type_traits"
#include "tuple"

/**
 * The definition of the well-documented <utility> file
 *
 * http://en.cppreference.com/w/cpp/header/utility
 */

STL_BEGIN

/**
 * std::pair implementation
 */
template<typename T1, typename T2>
struct pair : public tuple<T1,T2>
{
  using tuple<T1,T2>::tuple;
};

STL_END
