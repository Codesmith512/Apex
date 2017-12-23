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

/* UInt Concatenation */
stack_string& stack_string::operator+=(uint32_t i)
{
  char int_chars[10];

  for(short s = 0; s < 10; ++s)
    int_chars[s] = '0';

  for(short s = 9; s < 10 && i; --s)
  {
    int_chars[s] = '0' + (i%10);
    i = i / 10;
  }

  short s = 0;
  while(int_chars[s] == '0' && s < 9) ++s;
  while(s < 10)
    push_back(int_chars[s++]);

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
