#include "stdio.hpp"

#define WIDTH 80
#define HEIGHT 25

namespace std
{
  /* Constructor */
  cout::cout(unsigned short cursor_x /* = 0 */, unsigned short cursor_y /* = 0 */)
  :cursor{cursor_x, cursor_y}
  {

  }

  /* Stream out operator */
  cout& cout::operator<<(const char* string)
  {
    char* vram_ptr = reinterpret_cast<char*>(0xb8000 + (WIDTH * 2 * cursor.y) + (2 * cursor.x));
    char format = 0x0f;
    for(const char* c = string; *c; ++c)
    {
      *(vram_ptr++) = *c;
      *(vram_ptr++) = format;
    }
    return *this;
  }
}