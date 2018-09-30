#include "putc_vga.h"

#include "puti.h"
#include "puts.h"

char vga_x = 0;
char vga_y = 0;
char vga_attributes = 0x07; // Light grey

__attribute__((fastcall)) void vga_set_xy(const char x, const char y)
{
  vga_x = x;
  vga_y = y;
}

__attribute__((fastcall)) void vga_set_attributes(const char attributes)
{
  vga_attributes = attributes;
}

__attribute__((fastcall)) void puti_vga(long n, char length)
{
  return puti_generic(n, length, putc_vga);
}

__attribute__((fastcall)) unsigned puts_vga(const char *str)
{
  return puts_generic(str, putc_vga);
}
