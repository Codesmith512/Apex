#pragma once

/* Kernel */
class page_manager;

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
   * Classic memory allocation
   *
   * @param size    The size of the allocation to make
   * @return        The start of the allocation
   */
  void* malloc(std::size_t size);

  /**
   * Aligned memory allocation
   * 
   * @param size        The size of the allocation to make
   * @param alignment   The alignment of the allocation (must be a power of 2 <= size)
   * @return            The start of the allocation
   */
  void* malloc(std::size_t size, std::size_t alignment)

  /**
   * Classic memory free
   *
   * @param ptr     The start of the allocation to free
   */
  void free(void* ptr);

private:
  /* Returns the page map handling a given pointer */
  static page_map* get_page_map(void* ptr);

  /* Returns the page map for a page index (page 1 = (*)0x00400000)  (or indx+bit for page_bitmap 0,1 = (*)0x00400000) */
  static page_map* get_page_map(uint8_t page);
  static page_map* get_page_map(uint8_t indx, uint8_t bit);

  /* Page manager to use */
  page_manager* pager;

  /* Page bitfield */
  uint32_t page_bitmap[256];
}
