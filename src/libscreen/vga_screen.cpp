#include "vga_screen"

#define HW_WIDTH 80
#define HW_HEIGHT 25

SCREEN_BEGIN

/* Helper function -- converts a coordinate to it's vRAM address */
char* vga_screen::coord::vram_addr() const
{
  return reinterpret_cast<char*>(0xb8000) +
         (((y * HW_WIDTH) + x) * 2);
}

/* Construction from Geometry */
vga_screen::vga_screen(const coord& _origin, const coord& _size)
  :cursor_stack({{0,0}})
  ,origin(_origin)
  ,size(_size)
  ,attrib(attrib_t::Color::WHITE, attrib_t::Color::BLACK)
{

}

/* Write a c-style string */
void vga_screen::write(char const* str)
{
  for(; *str; ++str)
    put(*str);
}

/* Scroll the display */
void vga_screen::scroll()
{
  /* Memcpy line up */
  for(unsigned short y = 1; y < size.y; ++y)
    for(unsigned short x = 0; x < size.x; ++x)
    {
      char* src = (coord(x,y) + origin).vram_addr();
      char* dst = (coord(x,y-1) + origin).vram_addr();
      dst[0] = src[0];
      dst[1] = src[1];
    }

  /* Clear lowest line in the screen */
  for(unsigned short x = 0; x < size.x; ++x)
  {
    char* dst = (coord(x,size.y-1) + origin).vram_addr();
    dst[0] = ' ';
    dst[1] = attrib;
  }
}

/* Stream operator */
vga_screen& vga_screen::operator<<(char const* str)
{
  write(str);
  return *this;
}

/* Moves the current cursor */
void vga_screen::move_cursor(const coord& pos)
{ cursor_stack.back() = pos; }

/* Push a copy cursor */
void vga_screen::push_cursor()
{ cursor_stack.push_back(cursor_stack.back()); }

/* Push a new cursor */
void vga_screen::push_cursor(const coord& pos)
{ cursor_stack.push_back(pos); }

/* Pops a cursor */
void vga_screen::pop_cursor()
{
  cursor_stack.pop_back();
  if(cursor_stack.empty())
    cursor_stack.push_back({0,0});
}

/* Sets the current attribute */
void vga_screen::set_attrib(attrib_t _attrib)
{ attrib = _attrib; }

/* Writes a single character at the cursor */
void vga_screen::put(char c)
{
  coord& cursor = cursor_stack.back();

  /* Bounds check and correct cursor */
  if(cursor.x >= size.x)
  {
    cursor.x = 2;
    ++cursor.y;
  }

  for(; cursor.y >= size.y; --cursor.y)
    scroll();

  /* Print character */
  switch(c)
  {
  case '\n':
    cursor.x = 0;
    ++cursor.y;  
    break;

  default:
    {
      char* p = (cursor + origin).vram_addr();
      p[0] = c;
      p[1] = attrib;
      ++cursor.x;
    }
  }
}

SCREEN_END
