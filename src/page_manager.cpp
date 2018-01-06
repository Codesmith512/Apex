#include "page_manager"

/* APEX */
#include "asm_helpers"

/* Assembly helper functions */
extern "C"
{
  void __asm_enable_paging(const page_manager::page_directory*);
  void __asm_disable_paging();
}

/* The current page manager */
static page_manager* current_manager = 0;

/**
 * Manages a single page directory
 */
class page_manager::page_directory
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

/* Page directory constructor */
page_manager::page_directory::page_directory()
{
  reset();
}

/* Reset */
void page_manager::page_directory::reset()
{
  present = false;
  write_access = false;
  user_access = false;
  write_through = false;
  cache_disabled = false;
  accessed = false;
  zero = 0;
  large_pages = true;
  ignored = 0;
  os_use = 0;
  padding = 0;
  table_ptr = 0;
}

/* Manage physical RAM page */
void* page_manager::page_directory::set_phys_address(void* p)
{
  uintptr_t ptr = reinterpret_cast<uintptr_t>(p);
  table_ptr = ptr >> 22;
  present = static_cast<bool>(p);
  return p;
}

void* page_manager::page_directory::get_phys_address() const
{
  return reinterpret_cast<void*>(table_ptr << 22);
}

/* Manage user-space access */
bool page_manager::page_directory::set_user_access(bool _user_access)
{
  return (user_access = _user_access);
}

bool page_manager::page_directory::can_user_access() const
{
  return user_access;
}

/* Manage write access */
bool page_manager::page_directory::set_write_access(bool _write_access)
{
  return (write_access = _write_access);
}

bool page_manager::page_directory::has_write_access() const
{
  return write_access;
}

/* Initialization */
void page_manager::init(page_directory* _directory)
{
  /* Reset the directory */
  directory = _directory;
  for(unsigned short s = 0; s < 1024; ++s)
    directory[s].reset();

  /* Mark all virtual memory as free */
  for(uint32_t i = 0; i < 256; ++i)
    vmem_map[i] = 0x00000000;

  /* Mark all physical memory as allocated */
  for(uint32_t i = 0; i < 256; ++i)
    pmem_map[i] = 0xffffffff;
}

/* Enable paging */
void page_manager::enable_paging()
{
  if(current_manager)
    current_manager->set_enabled(false);

  current_manager = this;
  enabled = true;
  update_paging();
}

/* Update paging */
void page_manager::update_paging()
{
  if(enabled)
    __asm_enable_paging(directory);
}

/* Disable paging */
void page_manager::disable_paging()
{
  if(current_manager == this)
  {
    __asm_disable_paging();
    current_manager = 0;
  }
  enabled = false;
}

/* Gets the current page manager */
page_manager* page_manager::get_current_manager()
{
  return current_manager;
}

/* Allocates a specific page */
void page_manager::alloc_page(void* v, void* p)
{
  alloc_virt_page(v);
  alloc_phys_page(p);

  uintptr_t virt = reinterpret_cast<uintptr_t>(v);
  page_directory& dir = directory[virt >> 22];
  dir.set_phys_address(p);
  dir.set_write_access(true);

  update_paging();
}

/* Allocates the next available page */
void* page_manager::alloc_page()
{
  /* Locate the next free page */
  uint16_t vindx = 0;
  uint8_t vbit = 0;
  for(; vindx < 256; ++vindx)
  {
    /* Scan virtual memory map for a free page */
    if(vmem_map[vindx] != 0xffffffff)
    {
      /* Locate bit */
      for(; vbit < 32; ++vbit)
        if(!(vmem_map[vindx] & (1 << vbit)))
          break;

      break;
    }
  }

  /* No available virtual pages */
  if(vindx >= 256)
    apex::__break();

  /* Locate the next free page */
  uint16_t pindx = 0;
  uint8_t pbit = 0;
  for(; pindx < 256; ++pindx)
  {
    /* Scan virtual memory map for a free page */
    if(pmem_map[pindx] != 0xffffffff)
    {
      /* Locate bit */
      for(; pbit < 32; ++pbit)
        if(!(pmem_map[pindx] & (1 << pbit)))
          break;

      break;
    }
  }

  /* No available physical pages */
  if(pindx >= 256)
    apex::__break();

  /* Allocate */
  uint32_t vpage = ((vindx * 32) + vbit);
  uint32_t ppage = ((pindx * 32) + pbit);
  void* vptr = reinterpret_cast<void*>(vpage * 0x400000);
  void* pptr = reinterpret_cast<void*>(ppage * 0x400000);
  alloc_virt_page(vptr);
  alloc_phys_page(pptr);

  directory[vpage].set_phys_address(pptr);
  directory[vpage].set_write_access(true);
  update_paging();

  return vptr;
}

/* Free's the given page */
void page_manager::free_page(void* page)
{
  uintptr_t vpage = reinterpret_cast<uintptr_t>(page) >> 22;
  free_virt_page(page);
  free_phys_page(directory[vpage].get_phys_address());
  directory[vpage].reset();
  update_paging();
}

/* Marks the given physical page as free */
void page_manager::free_phys_page(void* phys)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(phys) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  pmem_map[indx] = pmem_map[indx] & ~(1 << bit);
}

/* Marks the given physical page as allocated */
void page_manager::alloc_phys_page(void* phys)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(phys) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  pmem_map[indx] = pmem_map[indx] | (1 << bit);
}

/* Marks the given virtual page as free */
void page_manager::free_virt_page(void* virt)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(virt) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  vmem_map[indx] = vmem_map[indx] & ~(1 << bit);
}

/* Marks the given virtual page as allocated */
void page_manager::alloc_virt_page(void* virt)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(virt) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  vmem_map[indx] = vmem_map[indx] | (1 << bit);
}
