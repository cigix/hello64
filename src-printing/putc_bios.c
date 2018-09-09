#include "putc.h"

void putc_bios(const char c)
{
  asm ("int $0x10" : : "a" (0x0E00 | c));
}
