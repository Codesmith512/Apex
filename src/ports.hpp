#pragma once

/**
 * Defines some functions to interface with IO ports
 */
namespace ports
{
  /**
   * The functions provided via asm
   */
  extern "C"
  {
    /* Writes 8/16/32 bit values to the given port */
    void out_8(uint16_t port, uint8_t byte);
    void out_16(uint16_t port, uint16_t byte);
    void out_32(uint16_t port, uint32_t byte);

    /* Reads 8/16/32 bit values from the given port */
    uint8_t in_8(uint16_t port);
    uint16_t in_16(uint16_t port);
    uint32_t in_32(uint16_t port);

    /* Performs an IO cycle */
    void io_wait();
  }
}
