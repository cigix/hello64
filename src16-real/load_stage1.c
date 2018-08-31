#include "print_16.h"

extern char stage1_start, stage1_end;

void load_stage1()
{
  char sectors_number = (&stage1_end - &stage1_start) / 512;
  short ret;
  asm ("int $0x13"
       : "=a" (ret)
       : "a" (0x2 /* Read Sectors */ << 8 | sectors_number),
         "b" (0x7E00) /* Where to write them */,
         "c" (0x0 /* Cylinder number */ << 8 | 0x2 /* Sector number */),
         "d" (0x0 /* Head number */ << 8 | 0x0 /* Drive identifier */)
      );
  if (0xFF00 & ret)
    print_bios_16("Error loading stage1");
}
