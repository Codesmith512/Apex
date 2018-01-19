#pragma once

/* STL */
#include "algorithm"
#include "libstl"
#include "utility"

/* Apex */
#include <helpers>

STL_BEGIN

/**
 * @class std::array
 * Well documented std::array type
 *
 * http://en.cppreference.com/w/cpp/container/array
 */
template<typename T, std::size_t _size>
struct array
{
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;

  /**
   * Element access
   */
  constexpr reference at(size_type pos)
  {
    if(pos > _size)
      apex::__break();

    return cArray[pos]; 
  }
  constexpr const_reference at(size_type pos) const
  {
    if(pos > _size)
      apex::__break();
    
    return cArray[pos]; 
  }

  constexpr reference operator[](size_type pos)
  { return cArray[pos]; }

  constexpr const_reference operator[](size_type pos) const
  { return cArray[pos]; }

  constexpr reference front()
  {
    static_assert(_size, "Cannot use array::front() on array _size 0.");
    return cArray[0];
  }

  constexpr const_reference front() const
  {
    static_assert(_size, "Cannot use array::front() on array _size 0.");
    return cArray[0];
  }

  constexpr reference back()
  {
    static_assert(_size, "Cannot use array::back() on array _size 0.");
    return cArray[_size-1];
  }

  constexpr const_reference back() const
  {
    static_assert(_size, "Cannot use array::back() on array _size 0.");
    return cArray[_size-1];
  }

  constexpr pointer data()
  { return cArray; }


  constexpr const_pointer data() const
  { return cArray; }

  /**
   * Iterators
   * @incomplete -- no reverse iterators
   */
  iterator begin()
  { return iterator(cArray); }

  const_iterator cbegin() const
  { return const_iterator(cArray); }

  iterator end()
  { return iterator(cArray + _size); }

  const_iterator cend() const
  { return const_iterator(cArray + _size); }

  /**
   * Capacity
   */
  constexpr bool empty() const
  { return _size == 0; }

  constexpr size_type size() const
  { return size; }

  constexpr size_type max_size() const
  { return size(); }

  /**
   * Operations
   */
  void fill(const T& val)
  {
    for(T& v : *this)
      v = val;
  }

  void swap(array<T, _size>& other)
  {
    for(size_t i = 0; i < size(); ++i)
      std::swap((*this)[i], other[i]);
  }

  /* Provide support for 0-size arrays */
  T cArray[_size ? _size : 1];
};

/**
 * Lexicographic comparisons
 */

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator==(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{
  size_t s = lhs.size();
  if(rhs.size() != s)
    return false;

  for(size_t i = 0; i < s; ++i)
    if(!(lhs[i] == rhs[i]))
      return false;

  return true;
}

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator!=(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{ return !(lhs == rhs); }

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator<(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] < rhs[i]))
      return false;

  return lhs_s < rhs_s;
}

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator<=(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] <= rhs[i]))
      return false;

  return lhs_s <= rhs_s;
}

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator>(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] > rhs[i]))
      return false;

  return lhs_s > rhs_s;
}

template<typename T, std::size_t Ts, typename U, std::size_t Us>
bool operator>=(const array<T, Ts>& lhs, const array<U, Us>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] >= rhs[i]))
      return false;

  return lhs_s >= rhs_s;
}

/**
 * Non-member functions
 */
template<std::size_t I, typename T, std::size_t N>
constexpr T& get(array<T,N>& a)
{
  static_assert(I < N, "std::get(std::array) invoked with I >= N");
  return a[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr const T& get(const array<T,N>& a)
{
  static_assert(I < N, "std::get(std::array) invoked with I >= N");
  return a[I];
}

template<typename T, std::size_t N>
void swap(array<T,N>& lhs, array<T,N>& rhs)
{ lhs.swap(rhs); }

STL_END
