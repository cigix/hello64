#include <stdint.h>

#include "descriptor.h"
#include "ram.h"

// Linker script variables
extern char stage1_start, stage1_end;

uint64_t gdt[4] = {
  // Null segment
  0,
  // Code segment : where we are currently loaded
  BASE_ADDR(0LL) | LIMIT_ADDR(0xFFFFFFFFLL) |
    ACCESS(PRIVILEGE(0LL) | EXEC_XR | NONCONFORMING | SYSTEM) |
    FLAGS(GRAIN_4K | SIZE_32),
  // Data segment : where we are currently loaded
  BASE_ADDR(0LL) | LIMIT_ADDR(0xFFFFFFFFLL) |
    ACCESS(PRIVILEGE(0LL) | DATA_RW | EXPAND_UP | SYSTEM) |
    FLAGS(GRAIN_4K | SIZE_32),
  // Extra data segment : to be determined
  0
};

struct
{
  uint16_t size;
  uint32_t offset;
} gdtr = { sizeof (gdt) - 1, (uint32_t)&gdt };

// 0x534D4150, or "SMAP"
#define SMAP (('S' << 24) | ('M' << 16) | ('A' << 8) | 'P')

struct e820_entry data_e820;

void find_extra_data()
{
  uint32_t continuation = 0;
  uint32_t signature, size;


  uint64_t data_size = &stage1_end - &stage1_start;

  do
  {
    asm ("int $0x15"
         : // Outputs
           "=a" (signature), "=b" (continuation), "=c" (size)
         : // Inputs
           // gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Machine-Constraints.html
           "a" (0xE820), "b" (continuation), "c" (sizeof (struct e820_entry)),
           "d" (SMAP), "D" (&data_e820)
        );
    if (signature != SMAP)
    {
      break;
    }
    if (data_e820.type == 1 && data_e820.length_lo >= data_size)
    {
      gdt[3] = BASE_ADDR((uint64_t)data_e820.base_lo) |
        LIMIT_ADDR(data_size) |
        ACCESS(PRIVILEGE(0) | DATA_RW | EXPAND_UP | SYSTEM) |
        FLAGS(GRAIN_4K | SIZE_32);
      break;
    }
  } while (continuation);
}
