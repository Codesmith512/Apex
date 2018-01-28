#include "int"

#include <stdint.h>

/* An entry in the IDT */
struct idt_entry
{
  idt_entry()
  :zero(0)
  { }

  /* Manages the attribute fields */
  union attrib_t
  {
    /* Default to not present */
    attrib_t()
    :raw(0)
    { one = 1; }

    /* The fields */
    struct
    {
      unsigned char gate : 2;
      unsigned char one : 1;
      unsigned char pmode : 1;
      unsigned char storage_seg : 1;
      unsigned char privl : 2;
      unsigned char present : 1;
    };

    /* The raw value */
    uint8_t raw;
  };

  uint16_t addr_low;
  uint16_t code_selector;
  uint8_t zero;
  attrib_t attr;
  uint16_t addr_high;
};

/* The IDT */
idt_entry idt[256];

/* Declarations for functions in int.asm */
extern "C"
{
  void load_idt(void*);
  void int_debug();
}

/* Test interrupt setup */
void setup_interrupts()
{
  /* Setup 0x03 */
  uint32_t call = reinterpret_cast<uint32_t>(&int_debug);
  idt[3].addr_low = static_cast<uint16_t>(call & 0xffff);
  idt[3].code_selector = 0x08;
  idt[3].attr.gate = 0b10;
  idt[3].attr.pmode = 1;
  idt[3].attr.storage_seg = 0;
  idt[3].attr.privl = 0;
  idt[3].attr.present = 1;
  idt[3].addr_high = static_cast<uint16_t>((call >> 16) & 0xffff);

  /* Submit idt */
  load_idt(idt);
}