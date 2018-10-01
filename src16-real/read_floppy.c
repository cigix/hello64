#include "functions_16.h"

uint16_t read_floppy(uint16_t cylinder, uint8_t head, uint8_t sector,
                     uint8_t count, uint16_t to)
{
  uint16_t ret;
  asm ("int $0x13"
       : "=a" (ret)
       : "a" (0x0200 | count),
         "b" (to),
         "c" ((cylinder & 0xFF) << 8 | (cylinder & 0x300) >> 2
              | (sector & 0x3F)),
         "d" (head << 8) // Floppy #1 is drive 0x00
      );
  return ret;
}
