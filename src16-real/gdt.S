#include "descriptor.h"
  .section .rodata
  .global gdtr
gdtr:
  // Size
  .word gdt_end-gdt-1
  // Offset
  .long gdt

gdt:
  // Null segment
  .quad 0
  // Code segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | EXEC_XR | NONCONFORMING | SYSTEM) | \
        FLAGS(GRAIN_4K | SIZE_32)
  // Data segment
  .quad BASE_ADDR(0) | LIMIT_ADDR(0xFFFFFFFF) | \
        ACCESS(PRIVILEGE(0) | DATA_RW | EXPAND_UP | SYSTEM) | \
        FLAGS(GRAIN_4K | SIZE_32)
gdt_end:
