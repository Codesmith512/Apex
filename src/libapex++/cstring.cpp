#include "cstring"

STL_BEGIN

void* memcpy(void* d, void const* s, std::size_t count)
{
  char* dest = reinterpret_cast<char*>(d);
  char const* src = reinterpret_cast<char const*>(s);

  while(count--)
    *(dest++) = *(src++);

  return d;
}

STL_END
