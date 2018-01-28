#include "interrupt"

#include <cstdlib>
#include <cstring>
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

  void int_wrapper_f();
  extern uint32_t int_wrapper_s;
}

/* Blank interrupt setup */
void setup_interrupts()
{
  ///* Setup 0x03 */
  //uint32_t call = reinterpret_cast<uint32_t>(&int_debug);
  //idt[3].addr_low = static_cast<uint16_t>(call & 0xffff);
  //idt[3].code_selector = 0x08;
  //idt[3].attr.gate = 0b10;
  //idt[3].attr.pmode = 1;
  //idt[3].attr.storage_seg = 0;
  //idt[3].attr.privl = 0;
  //idt[3].attr.present = 1;
  //idt[3].addr_high = static_cast<uint16_t>((call >> 16) & 0xffff);

  /* Submit idt */
  load_idt(idt);
}

/* Interrupt registration */
void register_int(uint8_t vec, int_func func, bool is_int)
{
  /* Create a new callback handler */
  char* wrapper = reinterpret_cast<char*>(std::malloc(int_wrapper_s));

  /* Awwww yeah -- it's not every day you get to cast
     a function pointer to a char pointer! :D */
  char* origin_wrapper = reinterpret_cast<char*>(&int_wrapper_f);

  /* Copy wrapper */
  std::memcpy(wrapper, origin_wrapper, int_wrapper_s);

  /* Replace 0xdeadc0de with our code! */
  uint32_t indx = 0;
  while(wrapper[indx+0] != char(0xde) ||
        wrapper[indx+1] != char(0xc0) ||
        wrapper[indx+2] != char(0xad) ||
        wrapper[indx+3] != char(0xde))
    indx++;

  /* And you thought the reinterpret cast was bad xD */
  std::memcpy(wrapper+indx, &func, 4);

  /* Setup the idt "normally" */
  uint32_t call = reinterpret_cast<uint32_t>(wrapper);
  idt_entry& int_handle = idt[vec];

  int_handle.addr_low = static_cast<uint32_t>(call & 0xffff);
  int_handle.addr_high = static_cast<uint32_t>((call >> 16) & 0xffff);
  int_handle.code_selector = 0x08;

  int_handle.attr.gate = is_int ? 0b10 : 0b11;
  int_handle.attr.pmode = 1;
  int_handle.attr.storage_seg = 0;
  int_handle.attr.privl = 0;
  int_handle.attr.present = 1;

  return;
}