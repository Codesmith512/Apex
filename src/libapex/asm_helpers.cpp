#include "asm_helpers.hpp"

APEX_BEGIN

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

APEX_END