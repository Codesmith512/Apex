#pragma once

#include "declval"
#include "libstl"

#include "type_traits_basic"
#include "type_traits_function"
#include "type_traits_reference"

/**
 * The definition of the well-documented <type_traits> file
 *
 * http://en.cppreference.com/w/cpp/header/type_traits
 */

STL_BEGIN

/**
 * Used in SFINAE, if B is true, has a type equal to T
 */
template<bool B, typename T = void>
struct enable_if
{ };

template<typename T>
struct enable_if<true, T>
{ using type = T; };

/**
 * Evaluates to true_type if the given type has a valid destructor
 */
namespace detail
{
  template<typename T>
  class is_dest_helper
  {
  private:
    template<typename _T>
    static true_type test() { declval<_T>().~_T(); return true_type(); };
    static false_type test();
  public:
    using value = decltype(test<T>());
  };
}

template<typename T>
struct is_destructable : public detail::is_dest_helper<T>::value
{ };

/**
 * Tests for const/volatile
 */
template<typename>
struct is_const : public false_type
{ };

template<typename T>
struct is_const<T const> : public true_type
{ };

template<typename>
struct is_volatile : public false_type
{ };

template<typename T>
struct is_volatile<T volatile> : public true_type
{ };

/**
 * Functions to remove const/volatile
 */
template<typename T>
struct remove_const
{ using type = T; };

template<typename T>
struct remove_const<const T>
{ using type = T; };

template<typename T>
struct remove_volatile
{ using type = T; };

template<typename T>
struct remove_volatile<volatile T>
{ using type = T; };

template<typename T>
struct remove_cv
{ using type = typename remove_const<typename remove_volatile<T>::type>::type; };

/**
 * Functions to add const/volatile
 */
template<typename T>
struct add_const
{ using type = const T; };

template<typename T>
struct add_volatile
{ using type = volatile T; };

template<typename T>
struct add_cv
{ using type = typename add_const<typename add_volatile<T>::type>::type; };

/**
 * Evaluates to true_type if the given type is void
 */
template<typename>
struct is_void : public false_type
{ };

template<>
struct is_void<void> : public true_type
{ };

/**
 * Evaluates to true_type if the given type is an integral type
 */
template<typename>
struct is_integral : public false_type
{ };

template<>
struct is_integral<bool> : public true_type
{ };

template<>
struct is_integral<char> : public true_type
{ };

template<>
struct is_integral<unsigned char> : public true_type
{ };

template<>
struct is_integral<wchar_t> : public true_type
{ };

template<>
struct is_integral<char16_t> : public true_type
{ };

template<>
struct is_integral<char32_t> : public true_type
{ };

template<>
struct is_integral<unsigned short> : public true_type
{ };

template<>
struct is_integral<short> : public true_type
{ };

template<>
struct is_integral<unsigned int> : public true_type
{ };

template<>
struct is_integral<long> : public true_type
{ };

template<>
struct is_integral<unsigned long> : public true_type
{ };

template<>
struct is_integral<long long> : public true_type
{ };

template<>
struct is_integral<unsigned long long> : public true_type
{ };

/**
 * Evaluates to true_type if the given type is a floating point number
 */
template<typename>
struct is_floating_point : public false_type
{ };

template<>
struct is_floating_point<float> : public true_type
{ };

template<>
struct is_floating_point<double> : public true_type
{ };

template<>
struct is_floating_point<long double> : public true_type
{ };

/**
 * Extension of is_function for member functions
 */
namespace detail
{
  template<typename>
  struct is_mem_fn_ptr_helper : public false_type
  { };

  template<typename T, typename U>
  struct is_mem_fn_ptr_helper<T U::*> : public true_type
  { };
}

template<typename T>
struct is_member_function_pointer : detail::is_mem_fn_ptr_helper<typename std::remove_cv<T>::type>
{ };

/**
 * is_{enum,union,class} all require a little help from the compiler
 */
template<typename T>
struct is_enum : public bool_constant<__is_enum(T)>
{ };

template<typename T>
struct is_union : public bool_constant<__is_union(T)>
{ };

template<typename T>
struct is_class : public bool_constant<__is_class(T)>
{ };

/**
 * An arithmetic type is an int or a float type
 */
template<typename T>
struct is_arithmetic : public bool_constant<is_integral<T>::value || is_floating_point<T>::value>
{ };

/**
 * A fundamental type is an arithmetic type + void
 */
template<typename T>
struct is_fundamental : public bool_constant<is_arithmetic<T>::value || is_void<T>::value>
{ };

/**
 * Various types of pointers
 */
template<typename>
struct is_pointer : public false_type
{ };

template<typename T>
struct is_pointer<T*> : public true_type
{ };

namespace detail
{
  template<typename>
  struct is_mem_ptr_helper : public false_type
  { };

  template<typename T, typename U>
  struct is_mem_ptr_helper<T U::*> : public true_type
  { };
}

template<typename T>
struct is_member_pointer : public bool_constant<detail::is_mem_ptr_helper<typename remove_cv<T>::type>::value>
{ };

STL_END
