#pragma once

#include <stdint.h>

/**
 * Functions for controlling the Programmable Interrupt Controller
 */

namespace pic
{
  /**
   * @struct IRQ
   * @breif Human-Readable IRQs with implicit conversion to bytes
   *
   * Each IRQ is stored as a bit in a 16 bit field
   */
  struct irq_t
  {
    /* Readable enumeration */
    enum E
    {
      KEYBOARD = 0x2,

      ALL = 0xffff,
    } e;

    /* Conversion from int */
    irq_t(uint16_t i)
    :e(static_cast<E>(i))
    { }

    /* Conversion to int */
    operator uint16_t() { return static_cast<uint16_t>(e); }

    /* Returns true if the given IRQ is contained (all bits are set) */
    bool contains(irq_t& other)
    { return (*this & other) == other; }

    bool operator==(irq_t& other)
    { return contains(other); }

    bool operator!=(irq_t& other)
    { return !(*this == other); }
  };

  /**
   * Enables the PIC, mapping the IRQs to start at 0x20
   * Also disables all IRQs and hw interrupts
   */
  void initialize();

  /**
   * @param irq   The IRQs to enable
   */
  void unmask(irq_t irqs);

  /**
   * @param irq   The IRQs to disable
   */
  void mask(irq_t irqs);

  /**
   * @param irq   The IRQ to acknowledge (sends EOI to master + slave (if needed))
   */
  void acknowledge(irq_t irq);

  /**
   * @return The IRR status (16-bit field of which interrupts have been requested)
   */
  irq_t get_irr();

  /**
   * @return The ISR status (16-bit field of which interrupts are currently raised)
   */
  irq_t get_isr();
}
