#include "vga_screen"

SCREEN_BEGIN

/**
 * vga_screen definition
 */

/* Constructs a windows with a border! */
vga_screen::vga_screen(coord const& _origin, coord const& _size, std::string const& _title, vga_manager* _manager)
  :has_border(false)
  ,active(false)
  ,origin(_origin)
  ,size(_size)
  ,manager(_manager)
  ,title(_title)
  ,cursor_stack({{0,0}})
  ,attrib_stack({color::LIGHT_GRAY, color::BLACK})
  ,framebuffer(size.y, std::vector<vga_entry>(size.x, {' ', {color::LIGHT_GRAY, color::BLACK}}))
{
  /* Validate size */
  if(size.x > 2 && size.y > 2)
  {
    origin += coord{1,1};
    size -= coord{2,2};
    has_border = true;
  }
}

/* Write a c-style string */
vga_screen& vga_screen::write(char const* str)
{
  for(; *str; ++str)
    put(*str);

  manager->passive_update_all();

  return *this;
}

/* Write a c++ style string */
vga_screen& vga_screen::write(std::string const& str)
{ return write(str.c_str()); }

/* Re-draw the border */
void vga_screen::update_border()
{
  if(!has_border)
    return;

  /* The border's origin */
  coord border_origin = origin - coord{1,1};

  attrib_t attrib = active ? manager->get_active_border()
                           : manager->get_inactive_border();

  /* Helper function to plot a single character on the border */
  auto put = [this, &border_origin, &attrib](char c, unsigned short x, unsigned short y)
  {
    vram_addr(border_origin + coord{x,y})[0] = c;
    vram_addr(border_origin + coord{x,y})[1] = attrib;
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
  if(active)
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
    for(; index <= title.size() && index <=size.x; ++index)
      put(title[index-1], index, 0);

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

/* Scroll the display */
void vga_screen::scroll()
{
  /* Update framebuffer */
  framebuffer.erase(framebuffer.begin());
  framebuffer.emplace_back(size.x, vga_entry{' ', peek_attrib()});

  if(is_active())
  {
    /* Memcpy line up */
    for(unsigned short y = 1; y < size.y; ++y)
      for(unsigned short x = 0; x < size.x; ++x)
      {
        char* src = vram_addr(coord(x,y) + origin);
        char* dst = vram_addr(coord(x,y-1) + origin);
        dst[0] = src[0];
        dst[1] = src[1];
      }

    /* Clear lowest line in the screen */
    for(unsigned short x = 0; x < size.x; ++x)
    {
      char* dst = vram_addr(coord(x,size.y-1) + origin);
      dst[0] = ' ';
      dst[1] = peek_attrib();
    }
  }
}

/* Pops a cursor */
void vga_screen::pop_cursor()
{
  cursor_stack.pop_back();
  if(cursor_stack.empty())
    cursor_stack.push_back({0,0});
}

/* Pops an attribute */
void vga_screen::pop_attrib()
{
  attrib_stack.pop_back();
  if(attrib_stack.empty())
    attrib_stack.push_back({color::LIGHT_GRAY, color::BLACK});
}

/* Re-draws everything with the current attribute */
void vga_screen::flush_attrib()
{
  attrib_t attrib = peek_attrib();

  for(std::vector<vga_entry>& entries : framebuffer)
    for(vga_entry& e : entries)
      e.a = attrib;

  if(is_active())
    for(unsigned short x = 0; x < size.x; ++x)
      for(unsigned short y = 0; y < size.y; ++y)
        vram_addr({x,y})[1] = attrib;

  manager->passive_update_all();
}

/* Re-draws everything on the screen */
void vga_screen::flush()
{
  update_border();

  for(unsigned short x = 0; x < size.x; ++x)
    for(unsigned short y = 0; y < size.y; ++y)
    {
      vram_addr(origin + coord{x,y})[0] = framebuffer[y][x].c;
      vram_addr(origin + coord{x,y})[1] = framebuffer[y][x].a;
    }
}

/* Updates the active status */
void vga_screen::set_active(bool _active)
{
  if(active == _active)
    return;

  active = _active;
  if(active)
    flush();
}

/* VRAM from coordinate */
char* vga_screen::vram_addr(coord const& c)
{
  return reinterpret_cast<char*>(0xb8000) + 
         (((c.y * manager->get_width()) + c.x) * 2);
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

      framebuffer[cursor.y][cursor.x] = {c, peek_attrib()};

      if(is_active())
      {
        char* p = vram_addr(cursor + origin);
        p[0] = c;
        p[1] = peek_attrib();
      }

      ++cursor.x;
    }
  }
}

/**
 * vga_manager definition
 */

/* Default constructor */
vga_manager::vga_manager(coord const& hw_size)
:size(hw_size)
,active_border(color::WHITE, color::BLACK)
,inactive_border(color::DARK_GRAY, color::BLACK)
{
  /* Clear screen */
  for(unsigned short x = 0; x < size.x; ++x)
    for(unsigned short y = 0; y < size.y; ++y)
    {
      char* p = reinterpret_cast<char*>(0xb8000) + (((y * size.x) + x) * 2);
      p[0] = ' ';
    }
}

/* Destructor */
vga_manager::~vga_manager()
{
  for(vga_screen* screen : screens)
    delete screen;
}

/* Sets the active screen */
void vga_manager::set_active(vga_screen* screen)
{
  for(auto it = screens.begin();
      it != screens.end();)
  {
    if(*it == screen)
      it = screens.erase(it);
    else
      (*it++)->set_inactive();
  }

  if(screen)
  {
    screens.push_back(screen);
    screen->set_active();
  }
}

/* Creates a new screen */
vga_screen& vga_manager::create_screen(coord const& origin, coord const& size, std::string const& title)
{
  screens.insert(screens.begin(), new vga_screen(origin, size, title, this));
  return *screens.front();
}

/* Update active border color */
void vga_manager::set_active_border(attrib_t border)
{
  active_border = border;
  for(vga_screen* screen : screens)
    if(screen->is_active())
      screen->update_border();
}

/* Update inactive border color */
void vga_manager::set_inactive_border(attrib_t border)
{
  inactive_border = border;
  for(vga_screen* screen : screens)
    if(!screen->is_active())
      screen->update_border();
}

/* Updates every screen */
void vga_manager::update_all()
{
  for(vga_screen* screen : screens)
    screen->flush();
}

/* Calls update_all if passive_update is set */
void vga_manager::passive_update_all()
{
  if(passive_update)
    update_all();
}

SCREEN_END
