#include "Page_Manager.hpp"

/* APEX */
#include "asm_helpers.hpp"

/* Assembly helper functions */
extern "C"
{
  void __asm_enable_paging(const Page_Manager::Page_Directory*);
  void __asm_disable_paging();
}

/* The current page manager */
static Page_Manager* current_manager = 0;

/* Page directory constructor */
Page_Manager::Page_Directory::Page_Directory()
{
  reset();
}

/* Reset */
void Page_Manager::Page_Directory::reset()
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
void* Page_Manager::Page_Directory::set_phys_address(void* p)
{
  uintptr_t ptr = reinterpret_cast<uintptr_t>(p);
  table_ptr = ptr >> 22;
  present = static_cast<bool>(p);
  return p;
}

void* Page_Manager::Page_Directory::get_phys_address() const
{
  return reinterpret_cast<void*>((table_ptr << 22) & 0xffa00000);
}

/* Manage user-space access */
bool Page_Manager::Page_Directory::set_user_access(bool _user_access)
{
  return (user_access = _user_access);
}

bool Page_Manager::Page_Directory::can_user_access() const
{
  return user_access;
}

/* Manage write access */
bool Page_Manager::Page_Directory::set_write_access(bool _write_access)
{
  return (write_access = _write_access);
}

bool Page_Manager::Page_Directory::has_write_access() const
{
  return write_access;
}

/* Page manager constructor */
Page_Manager::Page_Manager(Page_Directory* _directory)
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

/* Reset */
void Page_Manager::reset(Page_Directory* _directory)
{
  /* Copypasta destructor */
  disable_paging();

  /* Copypasta constructor */
  directory = _directory;

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
Page_Manager::~Page_Manager()
{
  disable_paging();
}

/* Enable paging */
void Page_Manager::enable_paging()
{
  if(current_manager)
    current_manager->set_enabled(false);
  
  __asm_enable_paging(directory);
  current_manager = this;
  enabled = true;
}

/* Disable paging */
void Page_Manager::disable_paging()
{
  if(current_manager == this)
  {
    __asm_disable_paging();
    current_manager = 0;
  }
  enabled = false;
}

/* Gets the current page manager */
Page_Manager* Page_Manager::get_current_manager()
{
  return current_manager;
}

/* Allocates a specific page */
void Page_Manager::alloc_page(void* v, void* p)
{
  uintptr_t virt = reinterpret_cast<uintptr_t>(v);

  Page_Directory& dir = directory[virt >> 22];
  dir.set_phys_address(p);
  dir.set_write_access(true);

  if(enabled)
    __asm_enable_paging(directory);
}

/* Allocates the next available page */
void* Page_Manager::alloc_page()
{
  // Not implemented
  __break();
}

/* Free's the given page */
void Page_Manager::free_page(void* page)
{
  // Not implemented
  __break();
}

/* Marks the given physical page as free */
void Page_Manager::free_phys_page(void* phys)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(phys) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  pmem_map[indx] = pmem_map[indx] & ~(1 << bit);
}

/* Marks the given physical page as allocated */
void Page_Manager::alloc_phys_page(void* phys)
{
  uintptr_t page = reinterpret_cast<uintptr_t>(phys) >> 22;
  uint8_t bit = page % 32;
  uint16_t indx = page / 32;
  pmem_map[indx] = pmem_map[indx] | (1 << bit);
}
