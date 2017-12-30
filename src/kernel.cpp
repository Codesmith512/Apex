/* Kernel */
#include "multiboot2.hpp"
#include "page_manager.hpp"

/* APEX */
#include "helpers.hpp"
#include "stdio.hpp"
#include "stack_string.hpp"

/**
 * The kernel main function and entry point
 */
extern "C"
{
  [[noreturn]] void kernel_main(page_manager::page_directory* __page_directory, const Multiboot2::Tag_Entry* tags)
  {
    using string = apex::stack_string;

    /* Print initialization message */
    const char* msg = "Beginning kernel boot sequence.\n\n";
    apex::cout << msg;
    page_manager pager(__page_directory);

    /* Parse Multiboot Tags */
    apex::cout << "Parsing multiboot tags...\n";
    for(const Multiboot2::Tag_Generic* tag = tags->first();
        tag->type != 0; tag = tag->next())
    {
      apex::cout << (string("Tag [") + tag->type + "]... ").c_str();
      switch(tag->type)
      {
        /* Memory Map */
      case 6:
        {
          apex::cout << "Memory Map!\n";
          const Multiboot2::Tag_MemoryMap* mmap = reinterpret_cast<const Multiboot2::Tag_MemoryMap*>(tag);
          const Multiboot2::Tag_MemoryMap::Entry* entry = mmap->first_entry();
          for(unsigned int i = 0; i < mmap->entry_count(); ++i, entry = entry->next())
          {
            string s("  [");
            s += i;
            s += "] base: ";
            s += entry->get_base_ptr();

            uint32_t l = entry->get_length_32();
            void* l_p = reinterpret_cast<void*>(l);
            s += " length: ";
            s += l_p;

            s += " type: ";
            s += entry->get_type_32();

            s += "\n";
            apex::cout << s.c_str();

            if(entry->get_length_32() >= 0x400000)
              for(uintptr_t page = apex::ceil(entry->get_base_32(), static_cast<uint32_t>(0x400000));
                  page < entry->get_base_32() + entry->get_length_32();
                  page += 0x400000)
                pager.free_phys_page(page);
          }
        }
        break;

      default:
        apex::cout << "  Ignored.\n";
      }
    }

    /* Enable paging  */
    apex::cout << "Linear mapping kernel + vram page...";
    // Linear-address kernel page
    void* kernel_addr = reinterpret_cast<void*>(&kernel_main);
    pager.alloc_page(kernel_addr, kernel_addr);
    pager.enable_paging();
    apex::cout << " Done.";

    /* Stall */
    for(;;);
  }
}
