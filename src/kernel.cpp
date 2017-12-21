#include "stdio.hpp"
#include "stack_string.hpp"

/**
 * The kernel main function and entry point
 */
extern "C"
{
  [[noreturn]] void kernel_main(void)
  {
    /* Print classic hello world message */
    std::stack_string h = "Hello";
    h += " World";
    std::cout() << h.c_str();

    /* Stall */
    for(;;);
  }
}
