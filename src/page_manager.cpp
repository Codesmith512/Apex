#include "page_manager.hpp"

/* APEX */
#include "asm_helpers.hpp"

/* Assembly helper functions */
extern "C"
{
  void __asm_enable_paging(const page_manager::page_directory*);
  void __asm_disable_paging();
}

/* The current page manager */
static page_manager* current_manager = 0;

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
  return reinterpret_cast<void*>((table_ptr << 22) & 0xffa00000);
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

/* Page manager constructor */
page_manager::page_manager(page_directory* _directory)
  :directory(_directory)
{
  /* Reset the directory */
  for(unsigned short s = 0; s < 1024; ++s)
    directory[s].reset();

  /* Mark all virtual memory as free */
  for(uint32_t i = 0; i < 256; ++i)
    vmem_map[i] = 0x00000000;

  /* Mark all physical memory as allocated */
  for(uint32_t i = 0; i < 256; ++i)
    pmem_map[i] = 0xffffffff;
}

/* Page manager destructor */
page_manager::~page_manager()
{
  disable_paging();
}

/* Enable paging */
void page_manager::enable_paging()
{
  if(current_manager)
    current_manager->set_enabled(false);
  
  __asm_enable_paging(directory);
  current_manager = this;
  enabled = true;
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
  uintptr_t virt = reinterpret_cast<uintptr_t>(v);

  page_directory& dir = directory[virt >> 22];
  dir.set_phys_address(p);
  dir.set_write_access(true);

  if(enabled)
    __asm_enable_paging(directory);
}

/* Allocates the next available page */
void* page_manager::alloc_page()
{
  // Not implemented
  __break();
}

/* Free's the given page */
void page_manager::free_page(void* page)
{
  // Not implemented
  __break();
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
