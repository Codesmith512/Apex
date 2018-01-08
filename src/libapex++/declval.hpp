#pragma once

#include "libstl"
#include "type_traits_reference"

STL_BEGIN

/**
 * Standard declval declaration
 */
template<class T>
typename std::add_rvalue_reference<T>::type declval();

STL_END
