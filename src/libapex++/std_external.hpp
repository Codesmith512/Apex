#pragma once

#include "libstl"

STL_BEGIN

/**
 * Defines external function requirements
 */
extern "C++"
{

}

/**
 * Defines external C function requirements
 */
extern "C"
{
  /**
   * The classic malloc function
   *
   * @param size    The size of the allocation (in bytes)
   * @return        The start of the allocation
   */
  void* malloc(size_t size);

  /**
   * The classic malloc function, but with alignment rules
   *
   * @param align   The alignment of the allocation (in bytes)
   * @param size    The size of the allocation (in bytes)
   * @return        The start of the allocation
   */
  void* aligned_alloc(size_t align, size_t size);

  /**
   * The classic free function
   *
   * @param ptr     The start of the allocation to free
   */
  void free(void* ptr);
}

STL_END
