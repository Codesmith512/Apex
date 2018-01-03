#pragma once

/* STL */
#include <stdint.h>

/**
 * @class page_manager
 * @brief manages the paging system, using 4MiB pages
 */
class page_manager
{
public:

  /* A single page directory */
  class page_directory;

  /* NOT CONSTRUCTABLE */
  page_manager()                    = delete;
  page_manager(const page_manager&) = delete;
  page_manager(page_manager&&)      = delete;

  /* NOT ASSIGNABLE */
  page_manager&   operator=(const page_manager&)  = delete;
  void            operator=(page_manager&&)       = delete;

  /* NOT DESTRUCTABLE */
  ~page_manager() = delete;

  /**
   * Initializer method
   * -- All virtual ram starts off free
   * -- All physical RAM starts off allocated
   *
   * @param directory     The start of the 4KiB aligned page directory to manage
   */
  void init(page_directory* directory);

  /**
   * Enables paging if not already enabled
   * Configures the CPU to use this manager
   * Sets this as the current manager
   */
  void enable_paging();

  /**
   * Updates the CPU after changing the paging information,
   *   but only if this is the current manager.
   */
  void update_paging();

  /**
   * If this is the current manager
   * - Paging is disabled
   * - Current manager is set to null
   * Otherwise, this call has no effect.
   */
  void disable_paging();

  /* The currently enabled page manager -- null if none is enabled */
  static page_manager* get_current_manager();

  /**
   * Allocates and maps the specified virtual page 
   *   to the specified physical page
   * UB if the given virtual page is already allocated.
   * Also marks it as allocated. Can be free'd with free_page.
   * @param virt    The virtual page to map
   * @param phys    The physical page to map to
   */
  void alloc_page(void* virt, void* phys);

  /**
   * Allocates a single page and returns it
   * @return A pointer to the start of the allocated virtual page
   */
  void* alloc_page();

  /**
   * Frees the given page of memory
   * @param page    A pointer to the virtual page to free
   */
  void free_page(void* page);

  /**
   * Marks a portion of physical RAM as free,
   *   allowing it to be allocated again.
   * @param page    The address of the page to free
   */
  void free_phys_page(void* page);
  void free_phys_page(uintptr_t page)
  { free_phys_page(reinterpret_cast<void*>(page)); }

  /**
   * Marks a portion of physical RAM as allocated,
   *   removing it from the allocation pool.
   * @param page    The address of the page to free.
   */
  void alloc_phys_page(void* page);
  void alloc_phys_page(uintptr_t page)
  { alloc_phys_page(reinterpret_cast<void*>(page)); }

  /**
   * Marks a portion of virtual RAM as free,
   *   allowing it to be allocated again.
   * @param page    The address of the page to free
   */
  void free_virt_page(void* page);
  void free_virt_page(uintptr_t page)
  { free_phys_page(reinterpret_cast<void*>(page)); }

  /**
   * Marks a portion of virtual RAM as allocated,
   *   removing it from the allocation pool.
   * @param page    The address of the page to free.
   */
  void alloc_virt_page(void* page);
  void alloc_virt_page(uintptr_t page)
  { alloc_phys_page(reinterpret_cast<void*>(page)); }

  /** Access Methods */
  bool is_enabled() const { return enabled; }

private:

  /* The actual page directory listings */
  page_directory* directory;

  /* The virtual memory bitfield map (0=free) */
  uint32_t vmem_map[256];
  /* The physical memory bitfield map (0=free) */
  uint32_t pmem_map[256];

  /* True if this manager is enabled */
  bool set_enabled(bool e) { return enabled = e; }
  bool enabled;
};
