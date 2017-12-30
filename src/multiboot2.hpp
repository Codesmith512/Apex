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

    const Tag_Generic* first() const
    {
      intptr_t p = reinterpret_cast<intptr_t>(this);
      p += sizeof(Tag_Entry);
      return reinterpret_cast<Tag_Generic*>(p);
    }
  };

  /**
   * The specific tag for the Memory Map
   */
  struct Tag_MemoryMap : public Tag_Generic
  {
    struct Entry
    {
    public:
      uint32_t get_base_32() const
      { return static_cast<uint32_t>(base_addr); }

      void* get_base_ptr() const
      { return reinterpret_cast<void*>(get_base_32()); }

      uint32_t get_length_32() const
      { return static_cast<uint32_t>(length); }

      uint32_t get_type_32() const
      { return static_cast<uint32_t>(type); }

      const Entry* next() const
      { return this + 1; }

    private:
      uint64_t base_addr;
      uint64_t length;
      uint32_t type;
      uint32_t reserved;
    };

    uint32_t entry_size;
    uint32_t entry_version;

    const Entry* first_entry() const
    {
      intptr_t p = reinterpret_cast<intptr_t>(this);
      p += sizeof(Tag_MemoryMap);
      return reinterpret_cast<Entry*>(p);
    }

    uint32_t entry_count() const
    { return (size - sizeof(Tag_MemoryMap)) / entry_size; }
  };
};