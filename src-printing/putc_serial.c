#include "putc.h"

void putc_serial(const char c)
{
  asm ("out %%al, %%dx" : : "a" (c), "d" (0x3f8));
}
