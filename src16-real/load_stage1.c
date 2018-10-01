#include "functions_16.h"
#include "putc_bios.h"

extern char stage1_start, stage1_end;

void load_stage1()
{
  uint8_t sectors_number = (&stage1_end - &stage1_start) >> 9; // / 512
  uint16_t ret = read_floppy(0, 0, 2, sectors_number, (uint32_t)&stage1_start);
  if (0xFF00 & ret)
  {
    puts_bios("Error loading stage1: error 0x");
    puti_bios(ret >> 8, 2);
  }
  else
    puts_bios("Successfully loaded stage 1");
  putc_bios('\r');
  putc_bios('\n');
}
