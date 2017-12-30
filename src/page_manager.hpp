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

  /**
   * Manages a single page directory
   */
  class page_directory
  {
  public:
    /* Constructor */
    page_directory();

    /* Resets the directory to it's default value */
    void reset();

    /* Manages the pointer to physical RAM */
    void* set_phys_address(void*);
    void* get_phys_address() const;

    /* Manages whether or not user-space can access the page */
    bool set_user_access(bool);
    bool can_user_access() const;

    /* Manages the write permission for the page */
    bool set_write_access(bool);
    bool has_write_access() const;

  private:
    /* True if the page is present */
    unsigned present  : 1;
    /* True if the page is RW, false for R- */
    unsigned write_access : 1;
    /* True if user-space has access */
    unsigned user_access : 1;
    /* True to enable write-through caching, false for write-back */
    unsigned write_through : 1;
    /* True to disable caching the page in the TLB */
    unsigned cache_disabled : 1;
    /* Set to true on access */
    unsigned accessed : 1;
    /* Should always be 0 */
    unsigned zero : 1;
    /* True for 4MiB pages, false for 1MiB */
    unsigned large_pages : 1;
    /* Unused */
    unsigned ignored : 1;
    /* Free for OS use */
    unsigned os_use : 3;
    /* Unused to ensure 4MiB physical paging */
    unsigned padding : 10;
    /* Pointer to 4MiB physical page address */
    unsigned table_ptr : 10;
  };

  /**
   * Constructor
   * -- All virtual ram starts off free
   * -- All physical RAM starts off allocated
   *
   * @param directory     The start of the 4KiB aligned page directory to manage
   */
  page_manager(page_directory* directory);

  /* Destructor (does NOT free the directory) */
  ~page_manager();

  /**
   * Enables paging if not already enabled
   * Configures the CPU to use this manager
   * Sets this as the current manager
   */
  void enable_paging();

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
