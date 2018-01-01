/* Kernel */
#include "multiboot2.hpp"
#include "page_manager.hpp"

/* APEX */
#include "asm_helpers.hpp"
#include "helpers.hpp"
#include "stdio.hpp"
#include "stack_string.hpp"

/* Get a static pager with static memory & no constructor call */
static char pager_memory[sizeof(page_manager)];
static page_manager& pager = *reinterpret_cast<page_manager*>(pager_memory);

/**
 * The kernel main function and entry point
 */
extern "C"
{
  void kernel_init(page_manager::page_directory* page_dir, const multiboot2::tag_entry* tags)
  {
    using string = apex::stack_string;
    pager.init(page_dir);

    /* Print initialization message */
    apex::cout << ">>> Starting kernel initialization sequence >>>\n";

    /* Parse Multiboot Tags */
    apex::cout << "  Parsing multiboot tags...\n";
    for(const multiboot2::tag_generic* tag = tags->first();
        tag->type != 0; tag = tag->next())
    {
      apex::cout << (string("    Tag [") + tag->type + "]... ").c_str();
      switch(tag->type)
      {
        /* Memory Map */
      case 6:
        {
          apex::cout << "Memory Map!\n";
          const multiboot2::tag_memory_map* mmap = reinterpret_cast<const multiboot2::tag_memory_map*>(tag);
          const multiboot2::tag_memory_map::Entry* entry = mmap->first_entry();
          for(unsigned int i = 0; i < mmap->entry_count(); ++i, entry = entry->next())
          {
            string s("      [");
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
        apex::cout << "Ignored.\n";
      }
    }
    apex::cout << "  End of multiboot tags.\n\n";

    /* Enable paging  */
    apex::cout << "  Linear mapping kernel + vram page...";
    void* kernel_addr = reinterpret_cast<void*>(&kernel_init);
    pager.alloc_page(kernel_addr, kernel_addr);
    pager.enable_paging();
    apex::cout << " Done.\n";

    /* Complete! */
    apex::cout << "<<< Kernel initialization sequence complete. <<<\n";
  }
}
