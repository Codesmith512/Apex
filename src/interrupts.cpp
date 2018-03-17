#include "interrupts"

#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

/* An entry in the IDT */
struct idt_entry
{
  idt_entry()
  :addr_low(0)
  ,code_selector(0)
  ,zero(0)
  ,addr_high(0)
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

/* The IDT -- setup without constructor/destructor call */
static char idt_mem[sizeof(idt_entry) * 256];
static std::array<idt_entry, 256>& idt = *reinterpret_cast<std::array<idt_entry,256>*>(idt_mem);

/* Declarations for functions in int.asm */
extern "C"
{
  void load_idt(void*);

  void int_wrapper_f();
  extern uint32_t int_wrapper_s;
}

/* Blank interrupt setup */
void interrupts::setup()
{
  /* Initialize table */
  for(idt_entry& ie : idt)
    ie = idt_entry();

  /* Submit idt */
  load_idt(idt.data());
}

/* Interrupt registration */
void interrupts::add(uint8_t vec, int_func func, bool is_int)
{
  /* Create a new callback handler */
  char* wrapper = new char[int_wrapper_s];

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

/* De-register interrupt */
void interrupts::remove(uint8_t vec)
{
  idt_entry& int_handle = idt[vec];

  uint32_t call = static_cast<uint32_t>(int_handle.addr_high) << 16 |
                  static_cast<uint32_t>(int_handle.addr_low);

  delete[] reinterpret_cast<char*>(call);

  int_handle = {};
}
