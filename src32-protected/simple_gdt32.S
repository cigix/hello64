#include "descriptor.h"
  .section .rodata
  .global gdtr32
gdtr32:
  // Size
  .word gdt32_end-gdt32-1
  // Offset
  .long gdt32

gdt32:
  // Null segment
  .quad 0
  // Code segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | EXEC_XR | NONCONFORMING) | \
        FLAGS(GRAIN_4K | SIZE_32)
  // Data segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | DATA_RW | EXPAND_UP) | \
        FLAGS(GRAIN_4K | SIZE_32)
gdt32_end:
