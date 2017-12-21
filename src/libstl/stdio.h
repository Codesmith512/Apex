#pragma once

/**
 * @class cout
 * @brief Used for basic text output
 */
namespace std
{
  class cout
  {
  public:

    /* Default/Initialization Constructor */
    cout(unsigned short cursor_x = 0, unsigned short cursor_y = 0);

    /* Writes a standard C-style string */
    cout& operator<<(const char*);

  private:
    /* The cursor location */
    struct Cursor { unsigned short x, y; } cursor;
  };
};
