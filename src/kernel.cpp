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
    apex::stack_string h = "Hello";
    h += " World";
    apex::cout << h.c_str();

    /* Stall */
    for(;;);
  }
}
