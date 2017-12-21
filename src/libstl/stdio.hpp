#pragma once

#include "libstl.hpp"

STL_BEGIN

/**
* @class cout_t
* @brief The type used for basic text output
*/

class cout_t
{
public:
  /* Writes a standard C-style string */
  cout_t& operator<<(const char*);
};

static cout_t cout;

STL_END
