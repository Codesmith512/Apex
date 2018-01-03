#pragma once

/**
 * Defines external function requirements
 */
extern
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
   * The classic free function
   *
   * @param ptr     The start of the allocation to free
   */
  void free(void* ptr);
}