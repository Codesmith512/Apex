#pragma once

/* APEX */
#include "libapex"

/* Compiler */
#include <stdint.h>

APEX_BEGIN

/**
 * @class stack_string
 * @brief provides a basic string class for strings up to 128 characters
 */
class stack_string
{
public:
  /* Default/Initial Constructor */
  stack_string(const char* = "");

  /* Copy Constructor */
  stack_string(const stack_string& other);

  /* Concatenation */
  stack_string& operator+=(const stack_string& other);
  stack_string& operator+=(uint32_t i);
  stack_string& operator+=(void* p);

  /* Operator+ For all += types */
  template<typename T>
  stack_string operator+(T other) const
  { return stack_string(*this) += other; }

  /* Character extraction */
  char operator[](unsigned short pos) const { return data[pos]; }

  /* Size of the string (excluding null) */
  unsigned short size() const { return end; }

  /* Conversion to null-terminated char * */
  char* c_str() { return data; }
  const char* c_str() const { return data; }

  /* Allocated size of underlying memory */
  static constexpr unsigned short alloc_size = 128;

private:
  /* push_back with safety check */
  void push_back(char c);

  unsigned short end;
  char data[alloc_size];
};


APEX_END
