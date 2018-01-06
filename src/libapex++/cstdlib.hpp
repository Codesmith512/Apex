#pragma once

/* STL */
#include "libstl.hpp"
#include "cstddef.hpp"

/* STL - malloc+free */
#include "std_external.hpp"

STL_BEGIN

extern "C"
{
  /**
   * The malloc function, but clears memory (=0)
   *
   * @param size    The size of the allocation (in bytes)
   * @return        The start of the allocation
   */
  void* calloc(size_t size);
}

STL_END
