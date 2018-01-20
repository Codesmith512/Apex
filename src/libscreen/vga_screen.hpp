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
 * - The cursor is a marker for the next write operation
 * - The topmost entry on the cursor stack defines the currently used cursor
 *
 * Attribute
 * - An attribute defines a color setting
 * - The topmost entry on the attribute stack defines the currently used attribute
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

    coord operator+(coord const& o) const
    { return coord(x + o.x, y + o.y); }
    void operator+=(coord const& o)
    { *this = (*this + o); }

    coord operator-(coord const& o) const
    { return coord(x - o.x, y - o.y); }
    void operator-=(coord const& o)
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
      WHITE,
    };

    /* Constructs an attribute from two colors */
    attrib_t(Color fg = Color::WHITE, Color bg = Color::BLACK)
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
  vga_screen(coord const& origin, coord const& size);

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
  void move_cursor(coord const& pos);

  /**
   * Pushes a copy of the current cursor
   */
  void push_cursor();

  /**
   * Pushes a new cursor
   * @param pos   The new position for the cursor
   */
  void push_cursor(coord const& pos);

  /**
   * Restores the previous cursor
   *
   * Popping the last cursor, pushes a cursor of {0,0}
   */
  void pop_cursor();

  /**
   * Returns the current cursor
   */
  coord const& peek_cursor() const;

  /**
   * Pushes a copy of the current attribute
   */
  void push_attrib();

  /**
   * Pushes a new attribute
   * @param attrib  The new attribute
   */
  void push_attrib(attrib_t attrib);

  /**
   * Restores the previous attribute
   *
   * Popping the last attribute pushes the default attribute
   */
  void pop_attrib();

  /**
   * Returns the current attribute
   */
  attrib_t peek_attrib() const;

private:
  /* Returns the current vRAM address of the cursor */
  char* cursor_addr();

  /* Writes a single character */
  void put(char c);

  /* The cursor stack */
  std::vector<coord> cursor_stack;
  /* The attribute stack */
  std::vector<attrib_t> attrib_stack;
  /* Screen geometry */
  coord origin, size;
};

SCREEN_END
