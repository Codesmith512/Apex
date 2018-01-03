#include "mem_manager.hpp"

/* Kernel */
#include "page_manager.hpp"

/* 4MiB Pages */
static constexpr PAGE_SIZE = 0x00400000;

/* Largest allowed allocation */
static constexpr MAX_ALLOC = 0x00100000;

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

  /* Number of blocks in a page */
  static constexpr BLOCKS_PER_PAGE = PAGE_SIZE / sizeof(std::min_align_t);
  using block_t = std::min_align_t;

  /**
   * Initializes the page map
   */
  void init();

  /**
   * Makes an allocation of the given size (in bytes)
   * Returns nullptr on failure
   */
  void* malloc(std::size_t size);
  void* malloc(std::size_t size, std::size_t align);

  /**
   * Frees the given allocation
   */
  void* free(void* ptr);

private:
  /* The number of free blocks in this page */
  size_t free_blocks;

  /* The block map for allocations in this page */
  block_t block_map[BLOCKS_PER_PAGE];
};

