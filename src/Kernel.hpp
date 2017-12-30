#pragma once

/* Kernel */
#include "Multiboot2.hpp"

/* APEX */
#include "Page_Manager.hpp"

/**
 * @class kernel
 * @brief The kernel. Don't mess this up.
 */
class Kernel
{
public:
  /* Constructor */
  Kernel(Page_Manager::Page_Directory* page_dir, const Multiboot2::Tag_Entry* tags);

  /* Destructor */
  ~Kernel();

private:
  Page_Manager pager;
};
