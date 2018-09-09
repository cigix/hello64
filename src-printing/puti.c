#include "puti.h"

#include "putc.h"

static const char *hexstr = "0123456789ABCDEF";

static void puti(const long n, char length, const putc_function putc)
{
  while (length)
  {
    --length;
    int pos = 0xF & (n >> (length * 4));
    putc(hexstr[pos]);
  }
}

__attribute__((fastcall)) void puti_bios(long n, char length)
{
  return puti(n, length, putc_bios);
}

__attribute__((fastcall)) void puti_vga(long n, char length)
{
  return puti(n, length, putc_vga);
}
