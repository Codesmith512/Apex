#pragma once

/* Kernel */
class page_manager;

/* STL */
#include <cstddef>

/* Compiler */
#include <stdint.h>

/**
 * @class mem_manager
 * @brief The memory manager for the APEX kernel
 */
class mem_manager
{
  class page_map;
public:

  /* NOT CONSTRUCTABLE */
  mem_manager()                   = delete;
  mem_manager(const mem_manager&) = delete;
  mem_manager(mem_manager&&)      = delete;

  /* NOT ASSIGNABLE */
  mem_manager&  operator=(const mem_manager&) = delete;
  void          operator=(mem_manager&&)      = delete;

  /* NOT DESTRUCTABLE */
  ~mem_manager() = delete;

  /**
   * Initializes the memory manager
   *
   * @param pager   The page manager to use to alloc/free pages
   */
  void init(page_manager* pager);

  /**
   * Aligned memory allocation
   * 
   * @param size        The size of the allocation to make
   * @param alignment   The alignment of the allocation (must be a power of 2 <= size)
   *                    Default alignment rules still apply, so 0=default
   * @return            The start of the allocation
   */
  void* malloc(std::size_t size, std::size_t alignment = 0);

  /**
   * Classic memory free
   *
   * @param ptr     The start of the allocation to free
   */
  void free(void* ptr);

private:
  /* Returns the page map for a page index (page 1 = (*)0x00400000) */
  static page_map* get_page_map(uint8_t page);

  /* Page manager to use */
  page_manager* pager;

  /* Page bitfield */
  uint32_t page_bitmap[256];

  /* Tests a specific page index */
  bool test_page(uint32_t page);

  /* Allocates a page index */
  void alloc_page(uint32_t page);

  /* Frees a page */
  void free_page(uint32_t page);
};
