#include "asm_helpers.hpp"

void __debug()
{
#ifdef _DEBUG
  __asm_debugbreak();
#endif
}

void __break()
{
  for(;;)
    __asm_debugbreak();
}