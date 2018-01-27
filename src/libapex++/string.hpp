#pragma once

#include "cstddef"
#include "libstl"
#include "vector"

STL_BEGIN

/**
 * @class std::string
 * @brief A basic string class, based on the ISO std::basic_string<char>
 *
 * Note: As much of this class is left in the header as possible to later support a proper template version.
 *
 * http://en.cppreference.com/w/cpp/container/vector
 */
class string
{
public:
  /*
   * Member types
   */
  using value_type = char;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = char&;
  using const_reference = char const&;
  using pointer = char*;
  using const_pointer = char const*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  static constexpr size_type npos = static_cast<size_type>(-1);

  /**
   * Constructors, Destructors, and Assignment
   */

  /* Constructs an empty string */
  string()
  {
    push_null();
  }

  /* Constructs a string made of c count times */
  string(size_type count, char c)
  {
    reserve(count);
    push_null();

    while(count--)
      push_back(c);
  }

  /* Substring constructor */
  string(string const& other, size_type pos, size_type count = npos)
  {
    count = std::min(other.size(), count);
    reserve(count);
    push_null();

    for(size_type i = pos; i < count; ++i)
      push_back(other[i]);
  }

  /* From c-string constructor */
  string(char const* str)
  {
    size_type size = 0;
    for(char const* s = str; *s; ++s)
      ++size;

    reserve(size);
    push_null();
    for(char const* s = str; *s; ++s)
      push_back(*s);
  }

  /* Copy constructor */
  string(string const& other)
  {
    reserve(other.capacity());
    push_null();
    for(char c : other)
      push_back(c);
  }

  /* Move constructor */
  string(string&& other)
  :string()
  {
    swap(other);
  }

  /* Destructor */
  ~string()
  { }

  /* Assignment operators for any of the above constructors */
  template<typename T>
  string& assign(T other)
  {
    this->~string();
    new (this) string(other);
    return *this;
  }

  string& operator=(char const* other)
  { return assign(other); }
  string& operator=(string const& other)
  { return assign(other); }
  string& operator=(string&& other)
  { return assign(other); }

  /**
   * Element access
   */
  reference at(size_type pos)
  { return data_vec.at(pos); }
  const_reference at(size_type pos) const
  { return data_vec.at(pos); }

  reference operator[](size_type pos)
  { return data_vec[pos]; }
  const_reference operator[](size_type pos) const
  { return data_vec[pos]; }

  reference front()
  { return data_vec.front(); }
  const_reference front() const
  { return data_vec.front(); }

  reference back()
  { return data_vec.back(); }
  const_reference back() const
  { return data_vec.back(); }

  pointer data()
  { return data_vec.data(); }
  const_pointer data() const
  { return data_vec.data(); }

  pointer c_str()
  { return data(); }
  const_pointer c_str() const
  { return data(); }

  /**
   * Iterators
   */
  iterator begin()
  { return data_vec.begin(); }
  const_iterator begin() const
  { return data_vec.begin(); }
  const_iterator cbegin() const
  { return data_vec.cbegin(); }

  iterator end()
  { return data_vec.end() - 1; }
  const_iterator end() const
  { return data_vec.end() - 1; }
  const_iterator cend() const
  { return data_vec.cend() - 1; }

  /**
   * Capacity
   */
  bool empty()
  { return size() == 0; }

  size_type size() const
  { return data_vec.size() - 1; }
  size_type length() const
  { return size(); }

  void reserve(size_type cap)
  { data_vec.reserve(cap); }

  size_type capacity() const
  { return data_vec.capacity(); }

  void shrink_to_fit()
  { data_vec.shrink_to_fit(); }

  /**
   * String operations
   */
  void clear()
  {
    data_vec.clear();
    push_null();
  }

  /* Inserts c at index */
  iterator insert(iterator pos, char c)
  { return insert(pos, 1, c); }

  /* Inserts count copies of c at index */
  string& insert(size_type index, size_type count, char c)
  {
    insert(begin() + index, count, c);
    return *this;
  }

  /* Inserts count copies of c at index */
  iterator insert(iterator pos, size_type count, char c)
  {
    while(count--)
      pos = data_vec.insert(pos, c) + 1;

    return pos;
  }

  /* Inserts str at index */
  string& insert(size_type index, string const& str)
  {
    insert(begin() + index, str);
    return *this;
  }

  /* Inserts str at index */
  iterator insert(iterator pos, string const& str)
  {
    for(char c : str)
      pos = data_vec.insert(pos, c) + 1;
    return pos;
  }

  /* Removes at most count characters starting with index */
  string& erase(size_type index, size_type count = npos)
  {
    count = std::min(count, size() - index);
    while(count--)
      data_vec.erase(data_vec.begin() + index);

    return *this;
  }

  /* Vector erase functions */
  iterator erase(iterator pos)
  { return data_vec.erase(pos); }
  iterator erase(iterator begin, iterator end)
  { return data_vec.erase(begin, end); }

  /* Standard push_back/pop_back (accounts for null-terminator) */
  void push_back(char c)
  { data_vec.insert(end(), c); }
  void pop_back()
  { data_vec.erase(end()); }

  string& append(char c)
  {
    push_back(c);
    return *this;
  }

  /* Appends c count times */
  string& append(size_type count, char c)
  {
    while(count--)
      push_back(c);

    return *this;
  }

  /* Appends the given string */
  string& append(string const& str)
  {
    for(char c : str)
      push_back(c);

    return *this;
  }

  /* Appends the given substring (str[pos,count)) */
  string& append(string const& str, size_type pos, size_type count = npos)
  {
    count = min(str.size() - pos, count);
    for(size_type i = 0; i < count; ++i)
      push_back(str[pos + i]);

    return *this;
  }

  /* Appends the given c-style string */
  string& append(char const* str)
  {
    while(*str)
      push_back(*(str++));

    return *this;
  }

  /* Operator += for all of the above */
  template<typename T>
  string& operator+=(T t)
  { return append(t); }

  /* Comparison function */
  int compare(string const& other)
  {
    if(size() < other.size())
      return -1;
    if(size() > other.size())
      return 1;

    for(size_type i = 0; i < size(); ++i)
    {
      if((*this)[i] < other[i])
        return -1;
      if((*this)[i] > other[i])
        return 1;
    }

    return 0;
  }

  /* Substring */
  string substr(size_type pos = 0, size_type count = npos) const
  { return string(*this, pos, count); }

  /* Resize -- careful to preserve \0 */
  void resize(size_type size)
  { resize(size, '\0'); }

  void resize(size_type size, char c)
  {
    data_vec.resize(size + 1, c);
    if(data_vec.back() != '\0')
      data_vec.push_back('\0');
  }

  /* Swap */
  void swap(string& other)
  { data_vec.swap(other.data_vec); }

  /**
   * Lexicographic comparisons
   */
  bool operator==(string const& other)
  { return data_vec == other.data_vec; }
  bool operator!=(string const& other)
  { return data_vec != other.data_vec; }

  bool operator<(string const& other)
  { return data_vec < other.data_vec; }
  bool operator<=(string const& other)
  { return data_vec <= other.data_vec; }

  bool operator>(string const& other)
  { return data_vec > other.data_vec; }
  bool operator>=(string const& other)
  { return data_vec >= other.data_vec; }

private:
  /* Pushes the null-terminator if needed */
  void push_null()
  {
    if(data_vec.empty() || data_vec.back() != '\0')
      data_vec.push_back('\0');
  }

  /* The vector holding the actual string data */
  vector<char> data_vec;
};

/* String concatenation */
string operator+(string const& lhs, string const& rhs);
string operator+(string const& lhs, char const* rhs);
string operator+(char const* lhs, string const& rhs);
string operator+(string const& lhs, char rhs);
string operator+(char lhs, string const& rhs);

/* Int conversion to string */
string to_string(int val);

/* Unsigned int conversion to string */
string to_string(unsigned int val);

/* Long long conversion to string */
string to_string(long long val);

/* Unsigned long long conversion to string */
string to_string(unsigned long long val);

/* Pointer conversion to string */
string to_string(void* ptr);

STL_END
