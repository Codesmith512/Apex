#pragma once

#include "libstl"

STL_BEGIN

template<typename T>
class initializer_list
{
public:

  /* Empty constructor */
  initializer_list()
  :val(0), len(0)
  { }

  /* Number of elements */
  std::size_t size() const
  { return len; }

  /**
   * Iterators
   */
  const T* begin() const
  { return val; }

  const T* end() const
  { return val + size(); }

private:
  /* Who can call a private constructor? The compiler! */
  initializer_list(const T* _val, std::size_t _len)
  :val(_val)
  ,len(_len)
  { }

  /* Value of the list */
  const T* val;
  /* Size of the list */
  std::size_t len;
};

STL_END
