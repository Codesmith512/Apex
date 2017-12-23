#pragma once

/* Compiler */
#include <stdint.h>

/**
 * Provides C++ data structs for working with GNU's multiboot2 header
 */
namespace Multiboot2
{
  /**
   * A generic tag of unknown type
   */
  struct Tag_Generic
  {
    uint32_t type;
    uint32_t size;

    const Tag_Generic* next() const
    {
      intptr_t p = reinterpret_cast<intptr_t>(this);
      p += size;
      uint32_t padding = size % 8;
      if(padding)
        padding = 8 - padding;
      p += padding;
      return reinterpret_cast<Tag_Generic*>(p);
    }
  };

  /**
   * The entry/starting tag
   */
  struct Tag_Entry
  {
    uint32_t total_size;
    uint32_t reserved;

    const Tag_Generic* next() const
    {
      intptr_t p = reinterpret_cast<intptr_t>(this);
      p += sizeof(Tag_Entry);
      return reinterpret_cast<Tag_Generic*>(p);
    }
  };
};