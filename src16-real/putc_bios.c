#include "putc_bios.h"

#include "puti.h"
#include "puts.h"

FASTCALL void puti_bios(long n, char length)
{
  return puti_generic(n, length, putc_bios);
}

FASTCALL unsigned puts_bios(const char *str)
{
  return puts_generic(str, putc_bios);
}
