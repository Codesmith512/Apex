#pragma once

#include "libstl"

#include "type_traits"
#include "utility"

STL_BEGIN

/**
 * @class vector
 * The well defined std::vector class
 */
template<typename T>
class vector
{
public:
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

  /** Size+Value Initialization */
  vector(size_t count, const T& value = T())
  :vector()
  {
    resize(count);
    for(size_t i = 0; i < count; ++i)
      (*this)[i] = value;
  }

  /** Copy Constructor */
  vector(const vector<T>& other)
  {
    size_t elements = other.size();
    reserve(other.capacity());
    resize(elements);
    for(size_t i = 0; i < elements; ++i)
      (*this)[i] = other[i];
  }

  /** Move constructor */
  vector(const vector<T>&& other)
  :data_start(exchange(other.data_start, 0))
  ,data_last(exchange(other.data_last, 0))
  ,data_end(exchange(other.data_end, 0))
  {

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
    return data_start[pos].get();
  }

  const T& at(size_t pos) const
  {
    T* elem = data_start + pos;
    if(elem >= data_last)
      apex::__break();
    return data_start[pos].get();
  }

  /** @return the specified element */
  T& operator[](size_t pos)
  { return data_start[pos].get(); }

  const T& operator[](size_t pos) const
  { return data_start[pos].get(); }

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
    T* new_start = std::malloc(sizeof(T) * cap);
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
  enable_if<is_destructable<T>::value> clear()
  {
    for(size_t i = 0; data_start + i < data_last; ++i)
      data_start[i].get().~T();

    data_last = data_start;
  }

  /** Erases all elements in the vector (for primitives) */
  void clear()
  { data_last = data_start; }

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
  enable_if<is_destructable<T>::value> pop_back()
  {
    (--data_last)->~T();
  }

  void pop_back()
  {
    --data_last;
  }

  /** Resize the vector */
  void resize(size_t new_size)
  {
    /* Increase capacity if necessary */
    if(new_size > capacity())
      reserve(new_size);

    /* Continually push back empty values until new_size is met */
    while(size() < new_size)
      emplace_back();
  }

  /** Swap */
  void swap(const vector<T>& other)
  {
    data_start = exchange(other.data_start, data_start);
    data_last = exchange(other.data_start, data_last);
    data_end = exchange(other.data_start, data_end);
  }

private:
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

  for(size_t i = 0; i < std::min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] < rhs[i]))
      return false;

  return lhs_s < rhs_s;
}

template<typename T, typename U>
bool operator<=(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < std::min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] <= rhs[i]))
      return false;

  return lhs_s <= rhs_s;
}

template<typename T, typename U>
bool operator>(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < std::min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] > rhs[i]))
      return false;

  return lhs_s > rhs_s;
}

template<typename T, typename U>
bool operator>=(const vector<T>& lhs, const vector<U>& rhs)
{
  size_t lhs_s = lhs.size();
  size_t rhs_s = rhs.size();

  for(size_t i = 0; i < std::min(lhs_s, rhs_s); ++i)
    if(!(lhs[i] >= rhs[i]))
      return false;

  return lhs_s >= rhs_s;
}

/* Swap Specialization */
template<typename T>
void swap(vector<T>& lhs, vector<T>& rhs)
{ lhs.swap(rhs); }

STL_END
