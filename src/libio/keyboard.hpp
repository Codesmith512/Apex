#pragma once

/* STL */
#include <array>
#include <vector>

namespace io
{
  namespace keyboard
  {
    /**
     * @brief Enables keyboard features and callbacks
     */
    extern void enable();

    /**
     * @brief Disables keyboard features and callback
     */
    extern void disable();

    /**
     * @enum key
     * @brief An enumeration of all supported keys
     */
    enum key_t
    {
      /* Default */
      KEY_NULL = 0,

      /* Alphabet */
      KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
      KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
      KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
      KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

      /* Numbers */
      KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
      KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

      /* Modifiers */
      KEY_LSHIFT, KEY_RSHIFT,
      KEY_LCTRL, KEY_RCTRL,
      KEY_LALT, KEY_RALT,
    };

    /**
     * @struct event
     * @brief Represents a single key event
     */
    struct event_t
    {
      event_t()
      :ascii(0)
      ,key(KEY_NULL)
      ,modifiers({})
      ,type(NONE)
      { }

      /**
       * @enum Type
       * @brief The type of event
       */
      enum type
      {
        NONE,
        PRESSED,
        RELEASED,
        REPEAT,
      };

      /** @brief The ascii character (if available) */
      char ascii;
      /** @brief The enumeration for the key */
      key_t key;
      /** @brief Any modifiers present */
      std::array<key_t, 6> modifiers;
      /** @brief The type of event this is */
      type type;
    };

    /** Signature for an event callback */
    using event_callback = void(*)(event_t const&);

    /**
     * @param f     The function to call when events are recieved
     */
    extern void register_callback(event_callback f);

    /**
     * @param f     The function to stop calling
     */
    extern void deregister_callback(event_callback f);
  }
}
