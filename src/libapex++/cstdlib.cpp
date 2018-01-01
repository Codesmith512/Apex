#include "cstdlib.hpp"

STL_BEGIN

void* calloc(size_t count)
{
  char* p = reinterpret_cast<char*>(malloc(count));
  for(size_t i = 0; i < count; ++i)
    p[i] = 0;

  return p;
}

STL_END
