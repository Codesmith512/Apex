#pragma once

#include "libstl"

/**
 * Implementation of std::is_function
 * Shamelessly stolen from http://en.cppreference.com/w/cpp/types/is_function
 */

STL_BEGIN

// primary template
template<class>
struct is_function : std::false_type { };
 
// specialization for regular functions
template<class Ret, class... Args>
struct is_function<Ret(Args...)> : std::true_type {};
 
// specialization for variadic functions such as std::printf
template<class Ret, class... Args>
struct is_function<Ret(Args......)> : std::true_type {};
 
// specialization for function types that have cv-qualifiers
template<class Ret, class... Args>
struct is_function<Ret(Args...) const> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) volatile> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) const volatile> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) volatile> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const volatile> : std::true_type {};
 
// specialization for function types that have ref-qualifiers
template<class Ret, class... Args>
struct is_function<Ret(Args...) &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) const &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) volatile &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) const volatile &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) volatile &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const volatile &> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) const &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) volatile &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args...) const volatile &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) volatile &&> : std::true_type {};
template<class Ret, class... Args>
struct is_function<Ret(Args......) const volatile &&> : std::true_type {};

STL_END
