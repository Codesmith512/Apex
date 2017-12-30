#pragma once

/* Kernel */
#include "multiboot2.hpp"

/* APEX */
#include "page_manager.hpp"

/**
 * @class kernel
 * @brief The kernel. Don't mess this up.
 */
class kernel
{
public:
  /* Constructor */
  kernel(page_manager::page_directory* page_dir, const multiboot2::tag_entry* tags);

  /* Destructor */
  ~kernel();

private:
  page_manager pager;
};
