#pragma once

/* APEX */
#include "libapex.hpp"

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

APEX_END
