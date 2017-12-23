#pragma once

/**
 * This file contains the forward-declarations for the assembly helper functions
 */
extern "C"
{
  /**
   * Loops infinitely until cx == 0
   * Sets VGA(0,0) to solid blue while looping
   */
  void __debugbreak();
}