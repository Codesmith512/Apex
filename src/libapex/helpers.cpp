#include "helpers.hpp"

extern "C" void __asm_debug();
extern "C" void __asm_break();

APEX_BEGIN

void __debug()
{
#ifdef _DEBUG
  __asm_debug();
#endif
}

void __break()
{
  __asm_break();
}

APEX_END
