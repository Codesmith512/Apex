#pragma once

/* STL */
#include "algorithm"
#include "libstl"
#include "type_traits"
#include "tuple"

/* Utility Pieces */
#include "utility_forward"

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
struct pair
{
  /* Default constructor */
  constexpr pair()
  { }

  /* Copy+Conversion Constructor */
  template<typename U1, typename U2>
  constexpr pair(pair<U1,U2> const& other)
  :first(other.first)
  ,second(other.second)
  { }

  /* Copy-element Constructor */
  template<typename U1, typename U2>
  constexpr pair(U1 const& _first, U2 const& _second)
  :first(_first)
  ,second(_second)
  { }

  /* Move Constructor */
  template<typename U1, typename U2>
  constexpr pair(pair<U1,U2>&& other)
  :first(std::forward<U1>(other.first))
  ,second(std::forward<U2>(other.second))
  { }

  /* Move-element Constructor */
  template<typename U1, typename U2>
  constexpr pair(U1&& x, U2&& y)
  :first(std::forward<U1>(x))
  ,second(std::forward<U2>(y))
  { }

  /* Assignment Operators */
  template<typename U1, typename U2>
  pair<T1,T2>& operator=(pair<U1,U2> const& other)
  {
    first = other.first;
    second = other.second;
    return *this;
  }

  template<typename U1, typename U2>
  pair<T1,T2>& operator=(pair<U1,U2>&& other)
  {
    first = std::forward<U1>(other.first);
    second = std::forward<U2>(other.second);
    return *this;
  }

  /* Swap operation */
  template<typename U1, typename U2>
  void swap(pair<U1,U2>&  other)
  {
    std::swap(first, other.first);
    std::swap(second, other.second);
  }

  T1 first;
  T2 second;
};

/* Function to explicitly make a pair */
template<typename T1, typename T2>
pair<T1,T2> make_pair(T1 const& first, T2 const& second)
{ return {first, second}; }

/* Lexicographic comparisons */
template<typename T1, typename T2>
bool operator==(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{
  return lexi::equal(lhs.first, rhs.first) && 
         lexi::equal(lhs.second, rhs.second);
}

template<typename T1, typename T2>
bool operator!=(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{ return !(lhs == rhs); }

template <typename T1, typename T2>
bool operator<(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{
  if(lexi::not_equal(lhs.first, rhs.first))
    return lexi::less_than(lhs.first, rhs.first);

  return lexi::less_than(lhs.second, rhs.second);
}

template <typename T1, typename T2>
bool operator<=(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{
  if(lexi::not_equal(lhs.first, rhs.first))
    return lexi::less_or_equal(lhs.first, rhs.first);

  return lexi::less_or_equal(lhs.second, rhs.second);
}

template <typename T1, typename T2>
bool operator>(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{
  if(lexi::not_equal(lhs.first, rhs.first))
    return lexi::greater_than(lhs.first, rhs.first);

  return lexi::greater_than(lhs.second, rhs.second);
}

template <typename T1, typename T2>
bool operator>=(std::pair<T1,T2> const& lhs, std::pair<T1,T2> const& rhs)
{
  if(lexi::not_equal(lhs.first, rhs.first))
    return lexi::greater_or_equal(lhs.first, rhs.first);

  return lexi::greater_or_equal(lhs.second, rhs.second);
}

/* Swap Specialization */
template<typename T1, typename T2>
void std::swap(std::pair<T1,T2>& first, std::pair<T1,T2>& second)
{ first.swap(second); }

/* Tuple get */

namespace detail
{
  template<size_t index>
  struct pair_get_index_t {};

  template<size_t index, typename T1, typename T2>
  void pair_get_impl(pair<T1,T2>& p, pair_get_index<index>);

  template<typename T1, typename T2>
  auto& pair_get_impl(pair<T1,T2>& p, pair_get_index<0>)
  { return p.first; }
  template<typename T1, typename T2>
  auto const& pair_get_impl(pair<T1,T2> const& p, pair_get_index<0>)
  { return p.first; }

  template<typename T1, typename T2>
  auto& pair_get_impl(pair<T1,T2>& p, pair_get_index<1>)
  { return p.second; }
  template<typename T1, typename T2>
  auto const& pair_get_impl(pair<T1,T2> const& p, pair_get_index<1>)
  { return p.second; }
}

template<size_t index, typename T1, typename T2>
auto& std::get(std::pair<T1,T2>& p)
{ template detail::pair_get_impl<index>(p); }

/* Tuple_size helper */
template<typename T1, typename T2>
class tuple_size<pair<T1,T2>> : public integral_constant<size_t, 2>
{ };

STL_END
