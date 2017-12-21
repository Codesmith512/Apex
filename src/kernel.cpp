#include "stdio.h"

/**
 * The kernel main function and entry point
 */
extern "C"
{
  [[noreturn]] void kernel_main(void)
  {
    /* Print classic hello world message */
    std::cout out;
    out << "Hello World!";

    /* Stall */
    for(;;);
  }
}
