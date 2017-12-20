/**
 * The kernel main function and entry point
 */
extern "C"
{
  [[noreturn]] void kernel_main(void)
  {
    /* Print a single "H" to the screen */
    char* ptr = reinterpret_cast<char*>(0xb8000);
    ptr[0] = 'H';
    ptr[1] = 0x07;

    /* Stall */
    for(;;);
  }
}
