/* Multiboot2 */
#include "multiboot2.hpp"

/* APEX */
#include "stdio.hpp"
#include "stack_string.hpp"

/**
 * The kernel main function and entry point
 */
extern "C"
{
  [[noreturn]] void kernel_main(const Multiboot2::Tag_Entry* tags)
  {
    /* Print initialization message */
    apex::cout << "Beginning kernel boot sequence.\n";

    /* Parse Multiboot2 Tags */
    apex::cout << "Parsing Multiboot2 tags (";
    apex::cout << (apex::stack_string() + tags->total_size).c_str();
    apex::cout << ")\n";
    unsigned int t = 0;
    const Multiboot2::Tag_Generic* tag = tags->next();
    for(;;)
    {
      apex::stack_string s;
      s += "[";
      s += t++;
      s += "] type: ";
      s += tag->type;
      s += " size: ";
      s += tag->size;
      s += "\n";
      apex::cout << s.c_str();

      if(!tag->type)
        break;

      tag = tag->next();
    }

    /* Stall */
    for(;;);
  }
}
