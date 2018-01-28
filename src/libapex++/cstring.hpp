#pragma once

#include "libstl"

/* STL */
#include "cstddef"

STL_BEGIN

/**
 * The well-defined memcpy function
 *
 * @param dest    Where to put copy
 * @param src     Where to copy from
 * @param count   Amount to copy
 */
void* memcpy(void* dest, void const* src, std::size_t count);

STL_END
