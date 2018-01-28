#pragma once

/**
 * Public functions from int.asm
 */
extern "C"
{
  void enable_int();
  void disable_int();
  void int_0x03();
}

/**
 * Sets up a basic interrupt table, with debug support
 */
void setup_interrupts();
