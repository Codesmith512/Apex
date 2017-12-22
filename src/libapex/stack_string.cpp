#include "stack_string.hpp"

STL_BEGIN

/* Initialize data to 0, then push string back */
stack_string::stack_string(const char* s /* = "" */)
  :end(0)
{
  for(unsigned short s = 0; s < alloc_size; ++s)
    data[s] = 0;

  while(*s)
    push_back(*(s++));
}

/* Copy */
stack_string::stack_string(const stack_string& other)
  :stack_string()
{
  (*this) += other;
}

/* Concatenation */
stack_string& stack_string::operator+=(const stack_string& other)
{
  for(unsigned short i = 0; i < other.size(); ++i)
    push_back(other[i]);
  return *this;
}

/* Safe push_back */
void stack_string::push_back(char c)
{
  data[end++] = c;
  if(end >= alloc_size - 2)
    end = 0;
}

STL_END
