#pragma once

/* Compiler */
#include <stdint.h>

namespace interrupts
{
  /**
   * Public functions from int.asm
   */
  extern "C"
  {
    /* Enables hardware based interrupts */
    void enable_hw_interrupts();
    /* Disables hardware based interrupts */
    void disable_hw_interrupts();

    /* Invokes interrupt vector 3 */
    void int_0x03();
  }

  /**
   * Sets up a standard 256-entry IDT
   * ...with no present interrupts
   */
  void setup();

  /**
   * The signature for an interrupt function
   */
  using int_func = void(*)(void);

  /**
   * Registers a given interrupt or trap
   *
   * Interrupts disable hw interrupts while processing
   * Traps do not
   *
   * @param vec       The interrupt vector to setup
   * @param func      Function to call for interrupt
   * @param is_int    True for an interrupt gate, false for trap
   */
  void add(uint8_t vec, int_func func, bool is_int);

  /**
   * De-registers the given interrupt vector
   */
  void remove(uint8_t vec);
}
