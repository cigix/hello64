#include "putc.h"

static char vga_x = 0;
static char vga_y = 0;
static char vga_attributes = 0x07; // Light grey

static short * const vga_text_buffer = (short*)0xB8000;

__attribute__((fastcall)) void vga_set_xy(const char x, const char y)
{
  vga_x = x;
  vga_y = y;
}

__attribute__((fastcall)) void vga_set_attributes(const char attributes)
{
  vga_attributes = attributes;
}

__attribute__((fastcall)) void putc_vga(const char c)
{
  if (c == '\n')
    ++vga_y;
  else if (c == '\r')
    vga_x = 0;
  else
  {
    vga_text_buffer[vga_y * 80 + vga_x] = (vga_attributes << 8) | c;
    ++vga_x;
  }
}
