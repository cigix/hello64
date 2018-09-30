#include "descriptor.h"
  .section .rodata
  .global gdtr64
gdtr64:
  // Size
  .word gdt64_end-gdt64-1
  // Offset
  .quad gdt64

gdt64:
  // Null segment
  .quad 0
  // Code segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | EXEC_XR | NONCONFORMING) | \
        FLAGS(GRAIN_4K | SIZE_32 | LONG)
  // Data segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | DATA_RW | EXPAND_UP) | \
        FLAGS(GRAIN_4K | SIZE_32 | LONG)
gdt64_end:
