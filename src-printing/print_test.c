#include "putc.h"
#include "puts.h"
#include "puti.h"

void print_test()
{
  vga_set_xy(0, 12);

  puts_bios("Hello World! (BIOS)\r\n\n");
  puts_vga("Hello World! (VGA)\r\n\n");

  puts_bios("Integer printing (BIOS) 0xDEADBEEF=");
  puti_bios(0xDEADBEEF, 8);

  puts_bios("Integer printing (VGA) 0xDECAF=");
  puti_bios(0xDECAF, 5);

  putc_serial('*');
}
