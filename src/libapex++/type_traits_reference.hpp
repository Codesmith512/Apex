#pragma once

#include "libstl"

STL_BEGIN

/**
 * Used to remove a reference from a type
 */
template<typename T>
struct remove_reference
{ using type = T; }

template<typename T>
struct remove_reference<T&>
{ using type = T; }

template<typename T>
struct remove_reference<T&&>
{ using type = T; }

/**
 * Evaluates to true_type if the given type is a reference
 */
template<typename>
struct is_reference : false_type
{ };

template<typename T>
struct is_reference<T&> : true_type
{ };

template<typename T>
struct is_reference<T&&> : true_type
{ };

/**
 * Evaluates to true_type if the given type is an lvalue reference
 */
template<typename>
struct is_lvalue_reference : false_type
{ };

template<typename T>
struct is_lvalue_reference<T&> : true_type
{ };

/**
 * Evaluates to true_type if the given type is an rvalue reference
 */
template<typename>
struct is_rvalue_reference : false_type
{ };

template<typename T>
struct is_rvalue_reference<T&&> : true_type
{ };

/**
 * Adds an lvalue/rvalue reference to the given type
 * @TODO Does not account for edgecase where references are not allowed
 */
template<typename T>
struct add_lvalue_reference
{
  using type = T&;
}

template<typename T>
struct add_rvalue_reference
{
  using type = T&&;
}

STL_END
