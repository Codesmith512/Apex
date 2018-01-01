#pragma once

/* STL */
#include "libstl.hpp"
#include "cstddef.hpp"

STL_BEGIN

extern "C"
{
  /**
   * FUNCTIONS DEFINED IN THE KERNEL
   */

  /**
   * The classic malloc function
   *
   * @param size    The size of the allocation (in bytes)
   * @return        The start of the allocation
   */
  void* malloc(size_t size);

  /**
   * The classic free function
   *
   * @param ptr     The start of the allocation to free
   */
  void free(void* ptr);

  /**
   * LOCALLY DEFINED FUNCTIONS
   */

  /**
   * The malloc function, but clears memory (=0)
   *
   * @param size    The size of the allocation (in bytes)
   * @return        The start of the allocation
   */
  void* calloc(size_t size);
}

STL_END
