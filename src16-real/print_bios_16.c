#include "print_16.h"

__attribute__((fastcall)) void print_bios_16(const char *str)
{
  short ax;
  while (*str)
  {
    ax = 0x0E00 + *str;

    asm ("int $0x10" : : "a" (ax));

    ++str;
  }
}
