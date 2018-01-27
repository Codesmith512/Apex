#pragma once

#include "libscreen"

/* STL */
#include <string>
#include <vector>

SCREEN_BEGIN
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
 * @enum color
 * @brief Enumerates the BIOS color codes
 */
enum class color : uint8_t
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

/**
 * @struct attrib
 * @brief a class to manage BIOS color attributes
 */
struct attrib_t
{
  /* Constructs an attribute from two colors */
  attrib_t(color fg = color::WHITE, color bg = color::BLACK)
  :val((static_cast<char>(bg) << 4) | static_cast<char>(fg))
  { }

  /* Constructs an attribute from a raw value */
  attrib_t(char a)
  :val(a)
  { }

  /* Implicit conversion to char */
  operator char() const { return val; }
  char val;
};

/**
 * @class vga_screen
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
class vga_manager;
class vga_screen
{
public:
  /**
   * @param origin    The upper-left most character
   * @param size      The size of the screen
   * @param title     The title of the screen
   * @param manager   The manager of this screen
   */
  vga_screen(coord const& origin, coord const& size, std::string const& title, vga_manager* manager);

  /**
   * Writes a c-style string at the cursor
   */
  vga_screen& write(char const* str);
  vga_screen& operator<<(char const* str)
    { return write(str); }

  /**
   * Writes a c++ string at the cursor
   */
  vga_screen& write(std::string const& str);
  vga_screen& operator<<(std::string const& str)
    { return write(str); }

  /**
   * Re-draws the border with the given attribute
   *
   * @param border_attrib   The attribute to draw the border with
   */
  void update_border();

  /**
   * Scrolls the display one line
   */
  void scroll();

  /**
   * Restores the previous cursor
   *
   * Popping the last cursor, pushes a cursor of {0,0}
   */
  void pop_cursor();

  /**
   * Restores the previous attribute
   *
   * Popping the last attribute pushes the default attribute
   */
  void pop_attrib();

  /**
   * Re-draws every character in the screen with the current attribute
   */
  void flush_attrib();

  /**
   * Sets the screen to active/inactive, updating the border if necessary
   */
  void set_active(bool _active = true);
  void set_inactive()
    { set_active(false); }

  /**
   * Trivial accessors 
   */
  bool is_active() const
    { return active; }

  std::string const& get_title() const
    { return title; }

  coord const& peek_cursor() const
    { return cursor_stack.back(); }

  attrib_t const& peek_attrib() const
    { return attrib_stack.back(); }

  /**
   * Trivial mutators
   */

  void push_cursor()
    { cursor_stack.push_back(cursor_stack.back()); }
  void push_cursor(coord const& pos)
    { cursor_stack.push_back(pos); }
  void move_cursor(coord const& pos)
    { cursor_stack.back() = pos; }

  void push_attrib()
    { attrib_stack.push_back(attrib_stack.back()); }
  void push_attrib(attrib_t attrib)
    { attrib_stack.push_back(attrib); }

  void set_title(std::string const& _title)
    { title = _title; }


private:
  /* Writes a single character, using the cursor_stack */
  void put(char c);

  /* True if this window has a border */
  bool has_border;
  /* True if this window is active */
  bool active;
  /* Screen geometry */
  coord origin, size;
  /* The manager this window belongs to */
  vga_manager* manager;
  /* The title of the window */
  std::string title;
  /* The cursor stack */
  std::vector<coord> cursor_stack;
  /* The attribute stack */
  std::vector<attrib_t> attrib_stack;
};

/**
 * @class vga_manager
 * @brief Manages a bunch of vga_screens
 *
 */
class vga_manager
{
public:

  /** Default constructor -- active border is WHITE on BLACK and inactive is DARK_GRAY on BLACK */
  vga_manager();

  /* Destructor */
  ~vga_manager();

  /**
   * @param screen  The screen to set active, nullptr for none
   */
  void set_active(vga_screen* screen);
  void set_active(vga_screen& screen)
    { set_active(&screen); }
  void set_active()
    { set_active(nullptr); }

  /**
   * Requests a new screen with the given parameters
   * (see vga_screen::vga_screen)
   * 
   * @param origin
   * @param size
   * @param title
   * @return  A reference to the new screen
   */
  vga_screen& create_screen(coord const& origin, coord const& size, std::string const& title);

  /**
   * Sets the active border, and updates all active windows
   */
  void set_active_border(attrib_t border);

  /**
   * Sets the inactive border, and updates all inactive windows
   */
  void set_inactive_border(attrib_t border);

  /**
   * Trivial accessors
   */
  attrib_t get_active_border() const
    { return active_border; }
  attrib_t get_inactive_border() const
    { return inactive_border; }

private:
  /* The active and inactive border attributes */
  attrib_t active_border, inactive_border;

  /* The windows owned by this manager */
  std::vector<vga_screen*> screens;
};

SCREEN_END
