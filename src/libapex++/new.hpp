#pragma once

/* STL */
#include "cstddef"
#include "libstl"

/* The type used for alignment (doesn't conflict with std::size_t) */
STL_BEGIN
enum class align_val_t : std::size_t {};
STL_END

/**
 * Replaceable allocation functions
 */
void* operator new(std::size_t count);
void* operator new[](std::size_t count);
void* operator new(std::size_t count, std::align_val_t align);
void* operator new[](std::size_t count, std::align_val_t align);

/**
 * NOT SUPPORTED: Replaceable non-throwing allocation functions
 * Reason: exceptions not supported yet anyway

void* operator new(std::size_t count, const std::nothrow_t&);
void* operator new[](std::size_t count, const std::nothrow_t&);
void* operator new(std::size_t count, std::align_val_t align, const std::nothrow_t&);
void* operator new[](std::size_t count, std::align_val_t align, const std::nothrow_t&);
  
  */

/**
 * Non-allocating placement functions
 */
void* operator new(std::size_t count, void* ptr);
void* operator new[](std::size_t count, void* ptr);

/**
 * Replaceable deallocation functions
 */
void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, std::align_val_t align);
void operator delete[](void* ptr, std::align_val_t align);
void operator delete(void* ptr, std::size_t size);
void operator delete[](void* ptr, std::size_t size);
void operator delete(void* ptr, std::size_t size, std::align_val_t align);
void operator delete[](void* ptr, std::size_t size, std::align_val_t align);


/**
 * NOT SUPPORTED: Replaceable non-throwing allocation functions
 * Reason: exceptions not supported yet anyway

void operator delete(void* ptr, const std::nothrow_t&);
void operator delete[](void* ptr, const std::nothrow_t&);
void operator delete(void* ptr, std::align_val_t align, const std::nothrow_t&);
void operator delete[](void* ptr, std::align_val_t align, const std::nothrow_t&);

  */

/**
 * Non-deallocating placement functions
 */
void operator delete(void* ptr, void*);
void operator delete[](void* ptr, void*);
