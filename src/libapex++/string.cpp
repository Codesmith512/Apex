#include "string"

/**
 * While most of the definition is in the header, until the class is fully template-d
 *  it is necessary to put the static functions here to prevent multiple definitions.
 */
STL_BEGIN

/* String concatenation */
string operator+(string const& lhs, string const& rhs)
{
  string s = lhs;
  s += rhs;
  return s;
}

/* String concatenation with c-style strings */
string operator+(string const& lhs, char const* rhs)
{
  string s = lhs;
  s += rhs;
  return s;
}

/* String concatenation with c-style strings */
string operator+(char const* lhs, string const& rhs)
{
  string s = lhs;
  s += rhs;
  return s;
}

/* Int conversion to string */
string to_string(int val)
{
  string str;

  bool negative = val < 0;
  if(negative)
    val *= -1;

  do
  {
    str.insert(static_cast<size_t>(0), 1, static_cast<char>(val % 10) + '0');
    val = val / 10;
  } while(val);

  if(negative)
    str.insert(0, '-');

  return str;
}

/* Unsigned int conversion to string */
string to_string(unsigned int val)
{
  string str;

  do
  {
    str.insert(static_cast<size_t>(0), 1, static_cast<char>(val % 10) + '0');
    val = val / 10;
  } while(val);

  return str;
}

/* Long long conversion to string */
string to_string(long long val)
{
  string str;

  bool negative = val < 0;
  if(negative)
    val *= -1;

  do
  {
    str.insert(static_cast<size_t>(0), 1, static_cast<char>(val % 10) + '0');
    val = val / 10;
  } while(val);

  if(negative)
    str.insert(0, '-');

  return str;
}

/* Unsigned long long conversion to string */
string to_string(unsigned long long val)
{
  string str;

  do
  {
    str.insert(static_cast<size_t>(0), 1, static_cast<char>(val % 10) + '0');
    val = val / 10;
  } while(val);

  return str;
}

/* Pointer conversion to string */
string to_string(void* ptr)
{
  string str;

  uintptr_t val = reinterpret_cast<uintptr_t>(ptr);

  do
  {
    char c = val % 16;
    val = val / 16;

    if(c < 10)
      c += '0';
    else
      c += 'a';

    str.insert(static_cast<size_t>(0), c);
  } while(val);

  return str;
}

STL_END
