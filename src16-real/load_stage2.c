#include "functions_16.h"
#include "putc_bios.h"

extern char stage1_start, stage1_end, stage2_start, stage2_end;

#define BYTES_PER_SECTOR 512
#define SECTORS_PER_PASS 127
#define BYTES_PER_PASS (BYTES_PER_SECTOR * SECTORS_PER_PASS)

void load_stage2()
{
  const unsigned stage0_size_sectors = 1;
  const unsigned stage1_size_bytes = (&stage1_end - &stage1_start);
  const unsigned stage1_size_sectors = stage1_size_bytes / BYTES_PER_SECTOR;
  const unsigned stage2_size_bytes = (&stage2_end - &stage2_start);
  const unsigned stage2_size_sectors = stage2_size_bytes / BYTES_PER_SECTOR;
  const unsigned read_to_segment = (unsigned)&stage1_end >> 4;

  unsigned read_from_sector = 1 + stage0_size_sectors + stage1_size_sectors;
  unsigned remaining_size_sectors = stage2_size_sectors;
  unsigned passes = stage2_size_sectors / SECTORS_PER_PASS;
  if (stage2_size_sectors % SECTORS_PER_PASS)
    ++passes;

  for (unsigned pass = 0; pass < passes; ++pass)
  {
    uint8_t sector_count = SECTORS_PER_PASS;
    if (remaining_size_sectors < SECTORS_PER_PASS)
      sector_count = remaining_size_sectors;
    puts_bios("load_stage2: pass ");
    puti_bios(pass + 1, 2);
    putc_bios('/');
    puti_bios(passes, 2);
    putc_bios('\r');
    putc_bios('\n');

    // es is (&stage1_end >> 4), bx is 0, so that es:bx is &stage1_end and the
    // whole segment is addressable with bx
    asm ("mov %0, %%es" : : "r"(read_to_segment));
    uint16_t ret = read_floppy(0, 0, read_from_sector, sector_count, 0);

    if (0xFF00 & ret)
    {
      puts_bios("Error loading stage2: error 0x");
      puti_bios(ret >> 8, 2);
      putc_bios('\r');
      putc_bios('\n');
      return;
    }
    if ((0xFF & ret) != sector_count)
    {
      puts_bios("Error loading stage2: wrong count: ");
      puti_bios(ret, 2);
      puts_bios(" != ");
      puti_bios(sector_count, 2);
      putc_bios('\r');
      putc_bios('\n');
      return;
    }

    to_unreal();

    asm ("mov %0, %%ds" : : "r"(read_to_segment));
    asm ("cld");
    asm ("rep movsd" : : "S" (0), "D" (0),
                         "c" (sector_count * BYTES_PER_SECTOR / 4));
    asm ("mov %0, %%ds" : : "r"(0));

    unset_unreal();

    read_from_sector += sector_count;
    remaining_size_sectors -= sector_count;
  }
  puts_bios("Successfully loaded stage 2\r\n");
}
