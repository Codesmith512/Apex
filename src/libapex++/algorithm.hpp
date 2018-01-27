#pragma once

/* STL */
#include "libstl"

STL_BEGIN

/* Helper functions for the operators, but only using < */
namespace lexi
{
  template<typename T1, typename T2>
  bool equal(T1 const& lhs, T2 const& rhs)
  { return !(lhs < rhs || rhs < lhs); }

  template<typename T1, typename T2>
  bool not_equal(T1 const& lhs, T2 const& rhs)
  { return !equal(lhs, rhs); }

  template<typename T1, typename T2>
  bool less_than(T1 const& lhs, T2 const& rhs)
  { return lhs < rhs; }

  template<typename T1, typename T2>
  bool less_or_equal(T1 const& lhs, T2 const& rhs)
  { return less_than(lhs,rhs) || equal(lhs,rhs); }

  template<typename T1, typename T2>
  bool greater_than(T1 const& lhs, T2 const& rhs)
  { return !less_than(lhs,rhs) && !equal(lhs,rhs); }

  template<typename T1, typename T2>
  bool greater_or_equal(T1 const& lhs, T2 const& rhs)
  { return !less_than(lhs,rhs); }
}

/* Min-Max implementations @todo non-branching integer representations */
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
