#pragma once

/* STL */
#include "libstl.hpp"

/* Compiler */
#include <stdint.h>

STL_BEGIN

/* Standard define symbols for c++ */
#define NULL 0

/* Type used for denoting sizes */
using size_t = uint32_t;
/* Type used for pointer arithmetic */
using ptrdiff_t = int32_t;
/* Type used for null pointers */
using nullptr_t = void*;
/* Scalar type with the largest alignment */
using max_align_t = long double;
/* Scalar type with the smallest alignment */
using min_align_t = uint32_t;

STL_END
