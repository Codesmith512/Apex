#pragma once


/**
 * @class tuple
 * 
 * The well-defined std::tuple class
 * http://en.cppreference.com/w/cpp/utility/tuple
 */
#include "libstl"

STL_BEGIN

/* Tuple with 3+ elements */
template<typename T, typename... Us>
class tuple
{
  /* A helper class to store the get-index as a template parameter */
  template<std::size_t i>
  struct get_helper{ };
public:
  tuple(const T& _t, const Us&... us)
  :t(_t)
  ,sub(us...)
  { }

  /*
   * Interface-functions for get() that return their implementation
   * This allows each _impl function to return a different type, but
   *  still deduce correctly
   */
  template<std::size_t indx>
  auto& get()
  { return get_impl(get_helper<indx>()); }
  template<std::size_t indx>
  auto const& get() const
  { return get_impl(get_helper<indx>()); }

private:
  /* Index OOB -- return subclass[indx-1] */
  template<std::size_t indx>
  auto& get_impl(get_helper<indx>)
  { return sub.template get<indx-1>(); }
  template<std::size_t indx>
  auto& get_impl(get_helper<indx>) const
  { return sub.template get<indx-1>(); }

  /* 0 requested -- return our value */
  T& get_impl(get_helper<0>)
  { return std::forward<T&>(t); }
  T const& get_impl(get_helper<0>) const
  { return std::forward<T const&>(t); }

  /* The value this tuple holds */
  T t;
  /* All other values are actually another tuple */
  tuple<Us...> sub;
};

/* Tuple with 2 elements */
template<typename T, typename U>
class tuple<T, U>
{
  /* A helper class to store the get-index as a template parameter */
  template<std::size_t i>
  struct get_helper{ };
public:
  tuple(const T& _t, const U& _u)
  :t(_t)
  ,u(_u)
  { }

  /*
   * Interface-functions for get() that return their implementation
   * This allows each _impl function to return a different type, but
   *  still deduce correctly
   */
  template<std::size_t indx>
  auto& get()
  { return get_impl(get_helper<indx>()); }
  template<std::size_t indx>
  auto const& get() const
  { return get_impl(get_helper<indx>()); }

private:

  /* Index OOB -- error (but with dummy return so that get() doesn't break) */
  template<std::size_t indx>
  void get_impl(get_helper<indx>);

  template<std::size_t indx>
  void get_impl(get_helper<indx>) const;

  /* 0 requested -- return our value */
  T& get_impl(get_helper<0>)
  { return std::forward<T&>(t); }
  T const& get_impl(get_helper<0>) const
  { return std::forward<T const&>(t); }

  /* 1 requested -- return our value */
  U& get_impl(get_helper<1>)
  { return std::forward<U&>(u); }
  U const& get_impl(get_helper<1>) const
  { return std::forward<U const&>(u); }

  T t;
  U u;
};

/* Tuple with 1 element */
template<typename T>
class tuple<T>
{
  /* A helper class to store the get-index as a template parameter */
  template<std::size_t i>
  struct get_helper{ };
public:
  tuple(const T& _t)
  :t(_t)
  { }

  /*
   * Interface-functions for get() that return their implementation
   * This allows each _impl function to return a different type, but
   *  still deduce correctly
   */
  template<std::size_t indx>
  auto& get()
  { return get_impl(get_helper<indx>()); }
  template<std::size_t indx>
  auto const& get() const
  { return get_impl(get_helper<indx>()); }

private:
  /* Index OOB -- error (but with dummy return so that get() doesn't break) */
  template<std::size_t indx>
  void get_impl(get_helper<indx>);

  template<std::size_t indx>
  void get_impl(get_helper<indx>) const;

  /* 0 requested -- return our value */
  T& get_impl(get_helper<0>)
  { return std::forward<T&>(t); }
  T const& get_impl(get_helper<0>) const
  { return std::forward<T const&>(t); }

  T t;
};

/* Non-member std::get */
template<std::size_t indx, typename... Ts>
auto& get(tuple<Ts...>& t)
{ return t.template get<indx>(); }

template<std::size_t indx, typename... Ts>
auto const& get(tuple<Ts...> const& t)
{ return t.template get<indx>(); }

/* Convenience make_tuple */
template<typename... Ts>
auto make_tuple(Ts... args)
{ return tuple<Ts...>(args...); }

/* Size of tuple */
template<typename T>
class tuple_size
{ }

template<typename... Ts>
class tuple_size<tuple<Ts...>> : public integral_constant<std::size_t, sizeof...(Ts)>
{ }

STL_END
