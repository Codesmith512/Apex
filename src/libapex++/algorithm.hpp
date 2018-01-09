#pragma once

/* STL */
#include "libstl"

STL_BEGIN

template<typename T>
const T& min(const T& lhs, const T& rhs)
{ return (lhs < rhs) ? lhs : rhs; }

template<typename T>
const T& min(const T& lhs, const T& rhs, auto comp)
{ return (comp(lhs, rhs)) ? lhs : rhs; }

template<typename T>
const T& max(const T& lhs, const T& rhs)
{ return (!(lhs < rhs)) ? lhs : rhs; }

template<typename T>
const T& max(const T& lhs, const T& rhs, auto comp)
{ return (!comp(lhs, rhs)) ? lhs : rhs; }

STL_END
