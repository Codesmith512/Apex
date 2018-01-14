#pragma once

/**
 * Provides the necessary ICXXABI (Itanium C++ Application Binary Interface) definitions.
 * Requires a heap and minimal C++ STL.
 */
extern "C"
{
  /**
   * Registers a destructor to be called on exit of the program
   * @param dtor    The destructor to call
   * @param obj     The object to invoke the destructor
   * @return        1 on success
   */
  int __cxa_atexit(void (*dtor)(void*), void* obj, void*);

  /**
   * If registered, invokes the destructor, and de-registers it
   * If not, does nothing
   *
   * @param obj     The object to destroy (null for everything)
   */
  void __cxa_finalize(void* obj);
}
