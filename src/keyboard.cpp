/* Kernel */
#include "interrupts"
#include "ports"
#include "pic"

/* IO */
#include <keyboard>

/* STL */
#include <vector>

namespace io
{
  namespace keyboard
  {
    /* Callbacks */
    static std::vector<event_callback> callbacks;

    /** Event handling */
    void int_handler()
    {
      event_t e;

      ports::in_8(0x60);
      pic::acknowledge(pic::irq_t::KEYBOARD);

      for(event_callback f : callbacks)
        f(e);
    }

    /** Keyboard setup */
    void enable()
    {
      /* Register keyboard ISR */
      interrupts::add(0x21, &int_handler, true);
      pic::unmask(pic::irq_t::KEYBOARD);
    }

    /** Keyboard cleanup */
    void disable()
    {
      /* Cleanup keyboard ISR */
      pic::mask(pic::irq_t::KEYBOARD);
      interrupts::remove(0x21);
    }

    /** Add a callback */
    void register_callback(event_callback f)
    {
      callbacks.push_back(f);
    }

    /** Remove a callback */
    void deregister_callback(event_callback f)
    {
      for(std::vector<event_callback>::iterator it = callbacks.begin();
          it != callbacks.end();
          )
      {
        if(*it == f)
          it = callbacks.erase(it);
        else
          ++it;
      }
    }
  }
}