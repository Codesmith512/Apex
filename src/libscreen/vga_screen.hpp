#pragma once

#include "libscreen"

/* STL */
#include <vector>

SCREEN_BEGIN

/**
 * @class screen
 * 
 * This class manages a single screen (analogous to a window in a modern OS)
 * 
 * Coordinates
 * - Defined as 2-component ushort's
 * - Origin at upper-left, x-horizontal, y-vertical
 *
 * Cursor
 * - Topmost entry on the stack defines the cursor's location
 */
class vga_screen
{
public:
  /**
   * @struct coord
   * @brief a convenience class for passing coordinates
   */
  struct coord
  {
    unsigned short x, y;

    coord(unsigned short _x, unsigned short _y)
    :x(_x)
    ,y(_y)
    { }

    coord(coord const& o)
    :coord(o.x, o.y)
    { }

    char* vram_addr() const;

    coord operator+(const coord& o) const
    { return coord(x + o.x, y + o.y); }
    void operator+=(const coord& o)
    { *this = (*this + o); }

    coord operator-(const coord& o) const
    { return coord(x - o.x, y - o.y); }
    void operator-=(const coord& o)
    { *this = (*this - o); }
  };

  /**
   * @struct attrib
   * @brief a class to manage BIOS color attributes
   */
  struct attrib_t
  {
    enum class Color : uint8_t
    {
      BLACK = 0,
      BLUE,
      GREEN,
      CYAN,
      RED,
      MAGENTA,
      BROWN,
      LIGHT_GRAY,
      DARK_GRAY,
      LIGHT_BLUE,
      LIGHT_GREEN,
      LIGHT_CYAN,
      LIGHT_RED,
      LIGHT_MAGENTA,
      YELLOW,
      WHITE
    };

    /* Constructs an attribute from two colors */
    attrib_t(Color fg, Color bg)
    :val((static_cast<char>(bg) << 4) | static_cast<char>(fg))
    { }

    /* Constructs an attribute from a raw value */
    attrib_t(char a)
    :val(a)
    { }

    /* Implicit conversion to char */
    operator char() { return val; }
    char val;
  };

  /**
   * @param origin  The upper-left most character
   * @param size    The size of the screen
   */
  vga_screen(const coord& origin, const coord& size);

  /**
   * Writes a c-style string at the cursor
   */
  void write(char const* str);
  vga_screen& operator<<(char const* str);

  /**
   * Scrolls the display one line
   */
  void scroll();

  /**
   * Moves the current cursor
   */
  void move_cursor(const coord& pos);

  /**
   * Pushes a copy of the current cursor
   */
  void push_cursor();

  /**
   * Pushes a new cursor
   * @param pos   The new position for the cursor
   */
  void push_cursor(const coord& pos);

  /**
   * Restores the previous cursor
   *
   * Popping the last cursor, pushes a cursor of {0,0}
   */
  void pop_cursor();

  /**
   * Sets the attribute byte
   * @param attrib -- the new attribute
   */
  void set_attrib(attrib_t attrib);

private:
  /* Returns the current vRAM address of the cursor */
  char* cursor_addr();

  /* Writes a single character */
  void put(char c);

  /* The cursor stack */
  std::vector<coord> cursor_stack;
  /* Screen parameters */
  coord origin, size;
  /* The attribute byte to write with */
  attrib_t attrib;
};

SCREEN_END
