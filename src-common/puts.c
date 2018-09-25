#include "puts.h"

#include "putc.h"

static unsigned puts(const char *str, const putc_function putc)
{
  unsigned count = 0;
  while (*str)
  {
    putc(*str);
    ++str;
    ++count;
  }
  return count;
}

__attribute__((fastcall)) unsigned puts_bios(const char *str)
{
  return puts(str, putc_bios);
}

__attribute__((fastcall)) unsigned puts_vga(const char *str)
{
  return puts(str, putc_vga);
}

__attribute__((fastcall)) unsigned puts_serial(const char *str)
{
  return puts(str, putc_serial);
}
