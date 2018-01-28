/* Kernel */
#include "mem_manager"
#include "multiboot2"
#include "page_manager"
#include "int"

/* Screen */
#include <vga_screen>

/* STL */
#include <array>
#include <std_external>
#include <tuple>
#include <vector>

/* APEX */
#include <helpers>
#include <stack_string>

/* Get a static pager with static memory & no constructor call */
static char pager_memory[sizeof(page_manager)];
static page_manager& pager = *reinterpret_cast<page_manager*>(pager_memory);

/* Get a static memory manager with static memory & no constructor call */
static char m_manager_memory[sizeof(mem_manager)];
static mem_manager& m_manager = *reinterpret_cast<mem_manager*>(m_manager_memory);

/**
 * The kernel initialization point
 * Supported Features
 * - Stack
 * - GDT
 *
 * Unsupported Features
 * - Paging (must setup)
 * - Heap (must setup)
 * - Global static variable initialization (happens after this function returns)
 */
extern "C" void kernel_init(page_manager::page_directory* page_dir, const multiboot2::tag_entry* tags)
{
  /* Initialize Paging */
  pager.init(page_dir);

  /* Parse Multiboot Tags */
  for(const multiboot2::tag_generic* tag = tags->first();
      tag->type != 0; tag = tag->next())
  {
    switch(tag->type)
    {
    case 6:
      {
        /* Read the memory map */
        const multiboot2::tag_memory_map* mmap = reinterpret_cast<const multiboot2::tag_memory_map*>(tag);
        const multiboot2::tag_memory_map::Entry* entry = mmap->first_entry();
        for(unsigned int i = 0; i < mmap->entry_count(); ++i, entry = entry->next())
        {
          /* Send to the pager */
          if(entry->get_length_32() >= 0x400000)
            for(uintptr_t page = apex::ceil(entry->get_base_32(), static_cast<uint32_t>(0x400000));
                page < entry->get_base_32() + entry->get_length_32();
                page += 0x400000)
              pager.free_phys_page(page);
        }
      }
      break;
    }
  }

  /* Enable paging  */
  void* kernel_addr = reinterpret_cast<void*>(&kernel_init);
  pager.alloc_page(kernel_addr, kernel_addr);
  pager.enable_paging();

  /* Enable the memory manager */
  m_manager.init(&pager);
}

/**
 * Kernel main function -- do whatever you want!
 * C++ is already as setup as it's going to get
 *
 * @return 0 on success, other on error
 */
extern "C" int kernel_main()
{
  /* Setup interrupts */
  setup_interrupts();

  /* invoke an interrupt */
  int_0x03();

  return 0;
}

/**
 * Define classic C functions for STL
 */
STL_BEGIN
extern "C"
{
  /* Malloc */
  void* malloc(size_t size)
  { return m_manager.malloc(size); }

  /* Aligned Malloc -- not sure why, but align comes first in std c */
  void* aligned_alloc(size_t alloc, size_t size)
  { return m_manager.malloc(size, alloc); }

  /* Free */
  void free(void* ptr)
  { m_manager.free(ptr); }
}
STL_END
