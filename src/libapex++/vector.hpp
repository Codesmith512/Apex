#pragma once

/* STL */
#include "algorithm"
#include "common_iterators"
#include "cstdlib"
#include "initializer_list"
#include "libstl"
#include "new"
#include "type_traits"
#include "utility"

/* APEX */
#include <helpers>

STL_BEGIN

/**
 * @class vector
 * The well defined std::vector class
 *
 * http://en.cppreference.com/w/cpp/container/vector
 */
template<typename T>
class vector
{
public:
  /*
   * Member types
   */
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = pointer;
  using const_iterator = const_pointer;

  /*
   * Constructors
   */

  /** Default(empty) constructor */
  vector()
  :data_start(0)
  ,data_last(0)
  ,data_end(0)
  {

  }

  /** Size initialization */
  vector(size_t count)
  :vector()
  {
    resize(count);
  }

  /** Size+Value Initialization */
  vector(size_t count, const T& value = T())
  :vector()
  {
    resize(count, value);
  }

  /** Copy Constructor */
  vector(const vector<T>& other)
  {
    size_t elements = other.size();
    reserve(other.capacity());
    for(size_t i = 0; i < elements; ++i)
      push_back(other[i]);
  }

  /** Move constructor */
  vector(const vector<T>&& other)
  :data_start(exchange(other.data_start, 0))
  ,data_last(exchange(other.data_last, 0))
  ,data_end(exchange(other.data_end, 0))
  {

  }

  /** Initializer list initialization */
  vector(std::initializer_list<T> init)
  {
    reserve(init.size());
    for(const T& t : init)
      push_back(t);
  }

  /**
   * Destructor
   */
  ~vector()
  {
    clear();
  }

  /*
   * Element Access Functions
   */

  /** @return the specified element (with bounds checking) */
  T& at(size_t pos)
  {
    T* elem = data_start + pos;
    if(elem >= data_last)
      apex::__break();
    return data_start[pos];
  }

  const T& at(size_t pos) const
  {
    T* elem = data_start + pos;
    if(elem >= data_last)
      apex::__break();
    return data_start[pos];
  }

  /** @return the specified element */
  T& operator[](size_t pos)
  { return data_start[pos]; }

  const T& operator[](size_t pos) const
  { return data_start[pos]; }

  /** @return the first element */
  T& front()
  { return at(0); }

  const T& front() const
  { return at(0); }

  /** @return the last element */
  T& back()
  { return at(size() - 1); }

  const T& back() const
  { return at(size() - 1); }

  /** @return the start of the allocated array */
  T* data()
  { return data_start; }

  const T* data() const
  { return data_start; }

  /*
   * Iterators
   * @incomplete -- missing reverse iterator support
   */
  iterator begin()
  { return iterator(data_start); }

  const_iterator cbegin() const
  { return const_iterator(data_start); }

  iterator end()
  { return iterator(data_last); }

  const_iterator cend() const
  { return const_iterator(data_last); }

  /*
   * Capacity Functions
   */

  /** @return true if the vector is empty */
  bool empty() const { return data_last == data_start; }

  /** @return the number of initialized elements */
  size_t size() const
  {
    return (reinterpret_cast<uintptr_t>(data_last) - 
            reinterpret_cast<uintptr_t>(data_start))
           / sizeof(T);
  }

  /** Increases the underlying allocation if cap > capacity() */
  void reserve(size_t cap)
  {
    if(cap <= capacity())
      return;

    /* New allocation */
    size_t s = size();
    T* new_start = reinterpret_cast<T*>(std::malloc(sizeof(T) * cap));
    T* new_last = new_start + s;
    T* new_end = new_start + cap;

    /* Move allocation */
    for(size_t i = 0; i < s; ++i)
      new_start[i] = std::move((*this)[i]);

    /* Free old allocation */
    std::free(data_start);

    /* Assign */
    data_start = new_start;
    data_last = new_last;
    data_end = new_end;
  }

  /** @return the number of allocated elements */
  size_t capacity() const
  {
    return (reinterpret_cast<uintptr_t>(data_end) - 
            reinterpret_cast<uintptr_t>(data_start))
           / sizeof(T);
  }

  /** Frees any unused capacity */
  void shrink_to_fit()
  {
    /* New allocation */
    size_t s = size();
    T* new_start = std::malloc(sizeof(T) * s);
    T* new_last = new_start + s;
    T* new_end = new_last;

    /* Move allocation */
    for(size_t i = 0; i < s; ++i)
      new_start[i] = std::move((*this)[i]);

    /* Free old allocation */
    std::free(data_start);

    /* Assign */
    data_start = new_start;
    data_last = new_last;
    data_end = new_end;
  }

  /*
   * Modification Functions
   */

  /** Erases all elements in the vector (for objects) */
  void clear()
  { clear_helper(*this); }

  /** Inserts an element */
  void insert(const iterator& it, const T& val)
  { emplace(it, val); }

  /** Constructs an element in-place */
  template<typename... Ctor_Args>
  void emplace(const iterator& it, Ctor_Args... args)
  {
    /* Make a reservation if necessary */
    if(size() == capacity())
      reserve(size() * 2 + 1);

    /* Move all elements after `it` back */
    for(T* ptr = data_last; ptr > it.get_ptr(); --ptr)
    {
      T* dst = ptr;
      T* src = ptr - 1;

      new (dst) T(std::move(*src));
      destroy(*src);
    }

    /* New element */
    ++data_last;

    /* Construct new element */
    new (it.get_ptr()) T(args...);
  }

  /** Erases a given element */
  void erase(const iterator& it)
  {
    /* Destroy given element */
    destroy(*it);

    /* Shift all elements forward */
    for(T* ptr = it.get_ptr(); ptr < data_last; ++ptr)
    {
      T* dst = ptr;
      T* src = ptr + 1;

      new (dst) T(std::move(*src));
      destroy(*src);
    }

    /* One less element */
    --data_last;
  }

  /** Pushes the given element back */
  void push_back(const T& val)
  { emplace_back(val); }

  /** Constructs a new element on the back */
  template<typename... Ctor_Args>
  void emplace_back(Ctor_Args... args)
  {
    /* Reserve new memory if needed */
    if(size() == capacity())
      reserve(size() * 2 + 1);

    /* Initialize at data_last and increment allocation */
    new (data_last++) T(args...);
  }

  /** Removes the last element */
  void pop_back()
  { pop_back_helper(*this); }

  /** Resize the vector */
  void resize(size_t new_size, const T& value = T())
  {
    /* Increase capacity if necessary */
    if(new_size > capacity())
      reserve(new_size);

    /* Continually push back values until new_size is met */
    while(size() < new_size)
      emplace_back(value);
  }

  /** Swap */
  void swap(vector<T>& other)
  {
    data_start = exchange(other.data_start, data_start);
    data_last = exchange(other.data_last, data_last);
    data_end = exchange(other.data_end, data_end);
  }

private:

  /* Helper function to use SFINAE properly */
  template<typename _T>
  static typename enable_if<is_destructable<_T>::value>::type clear_helper(vector<_T>& vec)
  {
    for(size_t i = 0; vec.data_start + i < vec.data_last; ++i)
      vec.data_start[i].~T();

    vec.data_last = vec.data_start;
  }

  template<typename _T>
  static typename enable_if<!is_destructable<_T>::value>::type clear_helper(vector<_T>& vec)
  { vec.data_last = vec.data_start; }

  /* Helper function to use SFINAE properly */
  template<typename _T>
  static typename enable_if<is_destructable<_T>::value>::type pop_back_helper(vector<_T>& vec)
  { (vec.data_last--)->~T(); }

  template<typename _T>
  static typename enable_if<!is_destructable<_T>::value>::type pop_back_helper(vector<_T>& vec)
  { --vec.data_last; }

  /* Invokes the destructor on an object if present, otherwise, does nothing */
  template<typename _T>
  static typename enable_if<is_destructable<_T>::value>::type destroy(_T& element)
  { element.~_T(); }

  template<typename _T>
  static typename enable_if<!is_destructable<_T>::value>::type destroy(_T& element)
  { }

  /* Start to the beginning of allocated data */
  T* data_start;
  /* Element-after the last initialized element */
  T* data_last;
  /* Element-after the last allocated element */
  T* data_end;
};

/*
 * Non-Member Functions
 */

template<typename T, typename U>
bool operator==(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t s = lhs.size();
  if(rhs.size() != s)
    return false;

  for(size_t i = 0; i < s; ++i)
    if(!(lhs[i] == rhs[i]))
      return false;

  return true;
}

template<typename T, typename U>
bool operator!=(const vector<T>& lhs, const vector<U>& rhs)
{ return !(lhs == rhs); }

template<typename T, typename U>
bool operator<(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] < rhs[i]))
      return false;

  return lhs_s < rhs_s;
}

template<typename T, typename U>
bool operator<=(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] <= rhs[i]))
      return false;

  return lhs_s <= rhs_s;
}

template<typename T, typename U>
bool operator>(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] > rhs[i]))
      return false;

  return lhs_s > rhs_s;
}

template<typename T, typename U>
bool operator>=(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] >= rhs[i]))
      return false;

  return lhs_s >= rhs_s;
}

/* Swap Specialization */
template<typename T>
void swap(vector<T>& lhs, vector<T>& rhs)
{ lhs.swap(rhs); }

STL_END
