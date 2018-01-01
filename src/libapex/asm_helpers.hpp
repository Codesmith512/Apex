#pragma once

/* APEX */
#include "libapex.hpp"

APEX_BEGIN

/**
 * This file contains the forward-declarations for the assembly helper functions
 */
extern "C"
{
  /**
   * Loops infinitely until cx == 0
   * Sets VGA(0,0) to solid blue while looping
   */
  void __asm_debugbreak();
  
  /**
   * Same as above, but only in debug mode
   */
  void __debug();

  /**
   * Infinite loop of breakpoints
   */
  [[noreturn]] void __break();
}

APEX_END
