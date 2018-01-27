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
vga_screen::vga_screen(coord const& _origin, coord const& _size)
  :has_border(false)
  ,origin(_origin)
  ,size(_size)
  ,cursor_stack({{0,0}})
  ,attrib_stack({attrib_t()})
{

}

/* Constructs a windows with a border! */
vga_screen::vga_screen(coord const& _origin, coord const& _size, std::string const& _title, attrib_t border_attrib, border_t thickness)
  :vga_screen(_origin, _size)
{
  /* Validate size */
  if(size.x > 2 && size.y > 2)
  {
    origin = origin + coord{1,1};
    size = size - coord{2,2};
    has_border = true;
  }
  else
    return;

  /* Validate Name */
  if(_title.size() < size.x)
    title = _title;

  /* Draw border with attribute */
  update_border(border_attrib, thickness);
}

/* Write a c-style string */
vga_screen& vga_screen::write(char const* str)
{
  for(; *str; ++str)
    put(*str);

  return *this;
}

/* Write a c++ style string */
vga_screen& vga_screen::write(std::string const& str)
{
  for(char c : str)
    put(c);

  return *this;
}

/* Re-draw the border */
void vga_screen::update_border(attrib_t attrib, border_t thickness)
{
  border_attrib = attrib;

  /* The border's origin */
  coord border_origin = origin - coord{1,1};

  /* Helper function to plot a single character on the border */
  auto put = [&border_origin, &attrib](char c, unsigned short x, unsigned short y)
  {
    (border_origin + coord{x,y}).vram_addr()[0] = c;
    (border_origin + coord{x,y}).vram_addr()[1] = attrib;
  };

  /*
   * The characters to draw with
   * [0] = Upper-left
   * [1] = Upper-right
   * [2] = Lower-left
   * [3] = Lower-right
   * [4] = Horizontal bar
   * [5] = Vertical bar
   */
  char palette[6];
  if(thickness == border_t::thick)
  {
    palette[0] = 201;
    palette[1] = 187;
    palette[2] = 200;
    palette[3] = 188;
    palette[4] = 205;
    palette[5] = 186;
  }
  else
  {
    palette[0] = 218;
    palette[1] = 191;
    palette[2] = 192;
    palette[3] = 217;
    palette[4] = 196;
    palette[5] = 179;
  }

  /* Top Row */
  {
    /* Upper left and right characters  */
    put(palette[0], 0, 0);
    put(palette[1], size.x + 1, 0);

    /* Plot the title */
    unsigned short index = 1;
    for(char c : title)
      put(c, index++, 0);

    /* Plot remaining title bar */
    for(; index <= size.x; ++index)
      put(palette[4], index, 0);
  }

  /* Bottom Row */
  {
    /* Lower left and right characters */
    put(palette[2], 0, size.y + 1);
    put(palette[3], size.x + 1, size.y + 1);

    /* Plot bar */
    for(unsigned short i = 1; i <= size.x; ++i)
      put(palette[4], i, size.y + 1);
  }

  /* Plot Left and Right Columns */
  for(unsigned short i = 1; i <= size.y; ++i)
  {
    put(palette[5], 0, i);
    put(palette[5], size.x+1, i);
  }
}

void vga_screen::update_border(border_t thickness)
{ update_border(border_attrib, thickness); }

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
    dst[1] = peek_attrib();
  }
}

/* Moves the current cursor */
void vga_screen::move_cursor(coord const& pos)
{ cursor_stack.back() = pos; }

/* Push a copy cursor */
void vga_screen::push_cursor()
{ cursor_stack.push_back(cursor_stack.back()); }

/* Push a given cursor */
void vga_screen::push_cursor(coord const& pos)
{ cursor_stack.push_back(pos); }

/* Pops a cursor */
void vga_screen::pop_cursor()
{
  cursor_stack.pop_back();
  if(cursor_stack.empty())
    cursor_stack.push_back({0,0});
}

/* Peeks the cursor */
vga_screen::coord const& vga_screen::peek_cursor() const
{ return cursor_stack.back(); }

/* Push a copy attribute */
void vga_screen::push_attrib()
{ attrib_stack.push_back(attrib_stack.back()); }

/* Push a given attribute */
void vga_screen::push_attrib(attrib_t attrib)
{ attrib_stack.push_back(attrib); }

/* Pops an attribute */
void vga_screen::pop_attrib()
{
  attrib_stack.pop_back();
  if(attrib_stack.empty())
    attrib_stack.push_back(attrib_t());
}

/* Peeks the attribute stack */
vga_screen::attrib_t vga_screen::peek_attrib() const
{ return attrib_stack.back(); }

/* Re-draws everything with the current attribute */
void vga_screen::flush_attrib()
{
  attrib_t attrib = peek_attrib();

  for(unsigned short x = 0; x < size.x; ++x)
    for(unsigned short y = 0; y < size.y; ++y)
      coord(x,y).vram_addr()[1] = attrib;
}

/* Writes a single character at the cursor */
void vga_screen::put(char c)
{
  coord& cursor = cursor_stack.back();

  /* Print character */
  switch(c)
  {
  case '\n':
    cursor.x = 0;
    ++cursor.y;  
    break;

  default:
    {
      /* Bounds check and correct cursor */
      if(cursor.x >= size.x)
      {
        cursor.x = 2;
        ++cursor.y;
      }

      for(; cursor.y >= size.y; --cursor.y)
        scroll();

      char* p = (cursor + origin).vram_addr();
      p[0] = c;
      p[1] = peek_attrib();
      ++cursor.x;
    }
  }
}

SCREEN_END
