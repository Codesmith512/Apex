#pragma once

#include "libstl"
#include "type_traits"

/**
 * A non-standard header defining some commonly-used iterators for outside use!
 */

STL_BEGIN

/**
 * Defines the RandomAccessIterator type for use with pointers
 *
 * @typename T    The value this iterates over
 * @bool isconst  True if this iterator is a const_iterator
 */
template<typename T, bool isConst /* = false */>
class random_access_iterator_ptr_t
{
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using pointer = T*;

  /**
   * Constructors
   */
  random_access_iterator_ptr_t(pointer p)
  :ptr(p)
  { }

  /** Converts a non-const to a const reference... */
  template<bool isOtherConst>
  random_access_iterator_ptr_t(
    typename std::enable_if<isConst || !isOtherConst, random_access_iterator_ptr_t<T, isOtherConst>>::type& other)
  :ptr(other.ptr)
  { }

  /** ...But not the other way */
  template<bool isOtherConst>
  random_access_iterator_ptr_t(
    typename std::enable_if<!(isConst || !isOtherConst), random_access_iterator_ptr_t<T, isOtherConst>>::type& other)
  = delete;

  /**
   * Requirements for ForwardIterator
   */

  /** Prefix increment */
  random_access_iterator_ptr_t<T, isConst> operator++()
  { return random_access_iterator_ptr_t(++ptr); }

  /** Postfix increment */
  random_access_iterator_ptr_t<T, isConst> operator++(int)
  { return random_access_iterator_ptr_t(ptr++); }

  /** Dereference */
  reference operator*()
  { return *ptr; }

  /** Equality Check */
  bool operator==(const random_access_iterator_ptr_t<T, isConst>& other)
  { return ptr == other.ptr; }

  /* Inequality Check */
  bool operator!=(const random_access_iterator_ptr_t<T, isConst>& other)
  { return !(*this == other); }

  /**
   * Requirements for BidirectionlIterator
   */

  /** Prefix decrement */
  random_access_iterator_ptr_t<T, isConst> operator--()
  { return random_access_iterator_ptr_t(--ptr); }

  /** Postfix decrement */
  random_access_iterator_ptr_t<T, isConst> operator--(int)
  { return random_access_iterator_ptr_t(ptr--); }

  /**
   * Requirements for RandomAccessIterator
   */

  /** Plus-Equal Operator */
  random_access_iterator_ptr_t<T, isConst>& operator+=(difference_type diff)
  {
    ptr += diff;
    return *this;
  }

  /** Minus-Equal Operator */
  random_access_iterator_ptr_t<T, isConst>& operator-=(difference_type diff)
  { return (*this) += (diff * -1); }

  /** Addition */
  random_access_iterator_ptr_t<T, isConst> operator+(difference_type diff)
  { return ptr + diff; }

  /** Subtraction */
  random_access_iterator_ptr_t<T, isConst> operator-(difference_type diff)
  { return ptr - diff; }

  /** Element access */
  reference operator[](difference_type diff)
  { return ptr[diff]; }

  /** Less Than */
  bool operator<(const random_access_iterator_ptr_t<T, isConst>& other)
  { return ptr < other.ptr; }

  /** Less Than or Equal to */
  bool operator<=(const random_access_iterator_ptr_t<T, isConst>& other)
  { return ptr <= other.ptr; }

  /** Greater Than */
  bool operator>(const random_access_iterator_ptr_t<T, isConst>& other)
  { return ptr > other.ptr; }

  /** Greater Than or Equal to */
  bool operator>=(const random_access_iterator_ptr_t<T, isConst>& other)
  { return ptr >= other.ptr; }

private:
  pointer ptr;
};

/** Addition */
template<typename T, bool isConst>
random_access_iterator_ptr_t<T, isConst> operator+(typename random_access_iterator_ptr_t<T, isConst>::difference_type diff, const random_access_iterator_ptr_t<T, isConst>& it)
{ return it + diff; }

/** Subtraction */
template<typename T, bool isConst>
random_access_iterator_ptr_t<T, isConst> operator-(typename random_access_iterator_ptr_t<T, isConst>::difference_type diff, const random_access_iterator_ptr_t<T, isConst>& it)
{ return it - diff; }

/** Specialization for const iterators */
template<typename T>
class random_access_iterator_ptr_t<T, true>
{
  using reference = const T&;
  using pointer = const T*;
};

/** More user-friendly types */
template<typename T>
using random_access_iterator = random_access_iterator_ptr_t<T, false>;

template<typename T>
using const_random_access_iterator = random_access_iterator_ptr_t<T, true>;

STL_END
