#pragma once

/* APEX */
#include "libapex"

APEX_BEGIN

/**
 * Computes the next multiple after a given number
 * @param num     The number to test
 * @param mult    The multiple to use
 * @return        The next multiple of mult after num (or num if it is a multiple)
 */
template<typename T>
constexpr T ceil(const T& num, const T& mult)
{
  T mod = num % mult;
  if(!mod)
    return num;
  return num + (mult - mod);
}

/**
 * Only works in debug -- calls an asm function that hangs until cx=0
 * Also sets the character at 0,0 to blue
 */
void __debug();

/**
 * Works all the time -- calls an asm function that hangs forever
 * Also sets the character at 0,0 to red
 */
void __break();

APEX_END
