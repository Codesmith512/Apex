/* Define the entry point */
ENTRY(_boot)

/* Define section layout */
SECTIONS
{
  /* Start at 1MiB */
  . = 1M;

  /* Next use multiboot + executable code */
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  /* R- data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /* RW data */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /* RW uninitialized */
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }
}