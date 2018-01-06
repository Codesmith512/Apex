#include "mem_manager"

/* Kernel */
#include "page_manager"

/* APEX */
#include <asm_helpers>
#include <helpers>

/* 4MiB Pages */
static constexpr std::size_t PAGE_SIZE = 0x00400000;

/* Largest allowed allocation */
static constexpr std::size_t MAX_ALLOC = 0x00100000;

/**
 * @class page_map
 * @brief Describes the allocation topology of the page
 */
class mem_manager::page_map
{
public:
  /* NOT CONSTRUCTABLE */
  page_map() = delete;
  page_map(const page_map&) = delete;
  page_map(page_map&&) = delete;

  /* NOT ASSIGNABLE */
  page_map& operator=(const page_map&) = delete;
  void operator=(page_map&&) = delete;

  /* NOT DESTRUCTABLE */
  ~page_map() = delete;

  /* Size of a single block */
  static constexpr uint32_t BLOCK_SIZE = alignof(std::min_align_t);
  /* Number of blocks in a page */
  static constexpr uint32_t BLOCKS_PER_PAGE = PAGE_SIZE / alignof(std::min_align_t);

  /* Number of elements in the block map */
  static constexpr uint32_t BLOCK_MAP_SIZE = BLOCKS_PER_PAGE / sizeof(uint32_t);
  /* Number of bits per element in the block map */
  static constexpr uint32_t BLOCK_MAP_BITS = 32;
  
  /* Number of blocks this objects takes up */
  /* Note: cannot use sizeof(...) with constexpr inside class.
     Also cannot use static constexpr outside class as class is private */
#define PAGE_MAP_RESERVED_BLOCKS (apex::ceil(sizeof(page_map), BLOCK_SIZE) / BLOCK_SIZE)

  /**
   * Initializes the page map
   */
  void init();

  /**
   * Makes an allocation of the given size and allocation (in bytes)
   * Returns nullptr on failure
   */
  void* malloc(std::size_t size, std::size_t align);

  /**
   * Frees the given allocation
   */
  void free(void* ptr);

private:
  /* The number of user-allocated blocks in this page */
  std::size_t allocated_blocks;

  /* The block map for allocations in this page */
  uint32_t block_map[BLOCK_MAP_SIZE];

  /* True if the block at the given index is allocated */
  bool test_block(uint32_t block);

  /* Allocates the given block */
  void alloc_block(uint32_t block);

  /* Frees the given block */
  void free_block(uint32_t block);
};

/* Initialize a page map */
void mem_manager::page_map::init()
{
  /* Free all blocks */
  allocated_blocks = 0;
  for(uint32_t i = 0; i < BLOCK_MAP_SIZE; ++i)
    block_map[i] = 0;

  /* Reserve blocks for page_map */
  for(uint32_t i = 0; i < PAGE_MAP_RESERVED_BLOCKS; ++i)
    alloc_block(i);
}

/* Aligned Malloc */
void* mem_manager::page_map::malloc(std::size_t size, std::size_t align)
{
  /* Cannot allocate */
  if(size > MAX_ALLOC || align > MAX_ALLOC)
    apex::__break();

  /* Validate alignment */
  {
    std::size_t min_align = 1;
    while(min_align <= size)
      min_align *= 2;
    min_align = min_align / 2;

    if(min_align > align)
      align = min_align;
  }

  /* Convert size and alignment to be in blocks */
  size = apex::ceil(size, BLOCK_SIZE) / BLOCK_SIZE;
  align = apex::ceil(align, BLOCK_SIZE) / BLOCK_SIZE;

  /* Scan blocks for suitable region */
  for(uint32_t i = PAGE_MAP_RESERVED_BLOCKS+1; i < BLOCKS_PER_PAGE; i += align)
  {
    /* Test size block */
    if(test_block(i-1))
      continue;

    /* Number of contiguous blocks */
    uint32_t contig = 0;
    while(test_block(i + contig) && contig < size - 1) ++contig;

    /* Suitable allocation found! */
    if(contig >= size - 1)
    {
      /* Allocate storage blocks */
      for(uint32_t j = 0; j < size; ++j)
        alloc_block(i + j);

      /* Allocate size blocks + store size */
      alloc_block(i-1);
      uintptr_t size_ptr = reinterpret_cast<uintptr_t>(this);
      size_ptr += (i-1) * BLOCK_SIZE;
      *reinterpret_cast<uint32_t*>(size_ptr) = size;

      /* Track allocations */
      allocated_blocks += size;

      /* Calculate return pointer */
      uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
      ptr += i * BLOCK_SIZE;
      return reinterpret_cast<void*>(ptr);
    }
  }

  /* Could not locate a block */
  return 0;
}

/* Free */
void mem_manager::page_map::free(void* ptr)
{
  /* Compute the block from the pointer */
  uint32_t block = reinterpret_cast<uint32_t>(ptr);
  block -= reinterpret_cast<uint32_t>(this);
  block = block / BLOCK_SIZE;

  /* Retrieve the size (in blocks) of the allocation to free */
  uint32_t size = *(reinterpret_cast<uint32_t*>(ptr)-1);

  /* Free size block */
  free_block(block-1);

  /* Free storage blocks */
  for(uint32_t i = 0; i < size; ++i)
    free_block(block + i);

  /* Track allocations */
  allocated_blocks -= size;
}

/* Test a block */
bool mem_manager::page_map::test_block(uint32_t block)
{
  return block_map[block / BLOCK_MAP_BITS] & (1 << (block % BLOCK_MAP_BITS));
}

/* Allocate a block */
void mem_manager::page_map::alloc_block(uint32_t block)
{
  block_map[block / BLOCK_MAP_BITS] |= (1 << (block % BLOCK_MAP_BITS));
}

/* Free a block */
void mem_manager::page_map::free_block(uint32_t block)
{
  block_map[block / BLOCK_MAP_BITS] &= ~(1 << (block % BLOCK_MAP_BITS));
}


/**
 * Implementation for mem_manager
 */

/* Initialization */
void mem_manager::init(page_manager* _pager)
{
  pager = _pager;

  /* Free all pages */
  for(uint32_t i = 0; i < 256; ++i)
    page_bitmap[i] = 0;
}

/* Malloc */
void* mem_manager::malloc(std::size_t size, std::size_t align)
{
  /* Try any/all allocated pages */
  for(uint32_t i = 0; i < 256; ++i)
  {
    if(!page_bitmap[i])
      continue;

    /* Try any valid page in the 32 bit range */
    uint32_t base_indx = i * 32;
    for(uint32_t offset = 0; offset < 32; ++offset)
    {
      if(test_page(base_indx + offset))
      {
        void* result = get_page_map(base_indx + offset)->malloc(size, align);
        if(result)
          return result;
      }
    }
  }

  /* Allocate a new page and return that */
  page_map* new_page = reinterpret_cast<page_map*>(pager->alloc_page());
  alloc_page(reinterpret_cast<uintptr_t>(new_page) / PAGE_SIZE);
  new_page->init();
  void* result = new_page->malloc(size, align);
  if(!result)
    apex::__break();
  return result;
}

/* Free */
void mem_manager::free(void* ptr)
{
  /* Get the page */
  uint32_t page = reinterpret_cast<uintptr_t>(ptr) / PAGE_SIZE;

  /* Safety check! */
  if(!test_page(page))
    apex::__break();

  /* Free the page */
  free_page(page);
  get_page_map(page)->free(ptr);
}

/* Page index to page map conversion */
mem_manager::page_map* mem_manager::get_page_map(uint8_t page)
{
  return reinterpret_cast<page_map*>(page * PAGE_SIZE);
}

/* Test page map */
bool mem_manager::test_page(uint32_t page)
{
  return page_bitmap[page / 32] & (1 << (page % 32));
}

/* Allocate page map */
void mem_manager::alloc_page(uint32_t page)
{
  page_bitmap[page / 32] |= (1 << (page % 32));
}

/* Free page map */
void mem_manager::free_page(uint32_t page)
{
  page_bitmap[page / 32] &= ~(1 << (page % 32));
}
