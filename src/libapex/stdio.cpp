#include "stdio.hpp"

#define WIDTH 80
#define HEIGHT 25

struct Cursor
{ unsigned short x, y; };
static Cursor cur {0,0};

APEX_BEGIN

/* Stream out operator */
cout_t& cout_t::operator<<(const char* string)
{
  char* vram_ptr = reinterpret_cast<char*>(0xb8000 + (WIDTH * 2 * cur.y) + (2 * cur.x));
  char format = 0x0f;
  for(const char* c = string; *c; ++c)
  {
    switch(*c)
    {
    case '\n':
      cur.x = WIDTH-1;
      break;

    default:
      *(vram_ptr++) = *c;
      *(vram_ptr++) = format;
    }

    ++cur.x;
    if(cur.x == WIDTH)
    {
      ++cur.y;
      cur.x = 0;
      if(cur.y == HEIGHT)
        cur.y = 0;
    }
  }
  return *this;
}

APEX_END
