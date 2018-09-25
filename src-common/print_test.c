#include "putc.h"
#include "puts.h"
#include "puti.h"

void print_test()
{
  vga_set_xy(0, 14);
  serial_setup();

  puts_bios("Hello World! (BIOS)\r\n");
  puts_vga("Hello World! (VGA)\r\n");
  puts_serial("Hello World! (serial)\n");

  puts_bios("Integer printing (BIOS) 0xDEADBEEF=");
  puti_bios(0xDEADBEEF, 8);
  putc_bios('\r');
  putc_bios('\n');

  puts_vga("Integer printing (VGA) 0xBAE=");
  puti_vga(0xCAFEBAE, 3);
  putc_vga('\r');
  putc_vga('\n');

  puts_serial("Integer printing (serial) 0xCAFEBABE=");
  puti_serial(0xCAFEBABE, 8);
  putc_serial('\n');
}
