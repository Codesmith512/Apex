#include <new>

/* STL */
#include <cstdlib>

/**
 * Replaceable allocation functions
 */
void* operator new(std::size_t count)
{ return std::malloc(count); }
void* operator new[](std::size_t count)
{ return std::malloc(count); }
void* operator new(std::size_t count, std::align_val_t align)
{ return std::aligned_alloc(static_cast<std::size_t>(align), count); }
void* operator new[](std::size_t count, std::align_val_t align)
{ return std::aligned_alloc(static_cast<std::size_t>(align), count); }

/**
 * Non-allocating placement functions
 */
void* operator new(std::size_t count, void* ptr)
{ return ptr; }
void* operator new[](std::size_t count, void* ptr)
{ return ptr; }

/**
 * Replaceable deallocation functions
 */
void operator delete(void* ptr)
{ return std::free(ptr); }
void operator delete[](void* ptr)
{ return std::free(ptr); }
void operator delete(void* ptr, std::align_val_t align)
{ return std::free(ptr); }
void operator delete[](void* ptr, std::align_val_t align)
{ return std::free(ptr); }
void operator delete(void* ptr, std::size_t size)
{ return std::free(ptr); }
void operator delete[](void* ptr, std::size_t size)
{ return std::free(ptr); }
void operator delete(void* ptr, std::size_t size, std::align_val_t align)
{ return std::free(ptr); }
void operator delete[](void* ptr, std::size_t size, std::align_val_t align)
{ return std::free(ptr); }

/**
 * Non-deallocating placement functions
 */
void operator delete(void* ptr, void*)
{ }
void operator delete[](void* ptr, void*)
{ }
