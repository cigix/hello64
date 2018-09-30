#ifndef PUTC_VGA_H
#define PUTC_VGA_H

#include <stdint.h>

extern char vga_x;
extern char vga_y;
extern char vga_attributes;

static inline void putc_vga(const char c)
{
  uint16_t * const vga_text_buffer = (uint16_t*)0xB8000;
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

void vga_set_xy(const char x, const char y) __attribute__((fastcall));
void vga_set_attributes(const char attributes) __attribute__((fastcall));

unsigned puts_vga(const char *str) __attribute__((fastcall));
void puti_vga(long n, char length) __attribute__((fastcall));

#endif /* !PUTC_VGA_H */
