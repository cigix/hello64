#include "descriptor.h"
  .code16
  .section .text
  .global to_unreal
to_unreal:
  push  %eax
  push  %bx
  cli

  lgdt  unreal_gdtr

  mov   %cr0, %eax
  or    $0x1, %al // Set PE
  mov   %eax, %cr0

  mov   $SEGMENT_SELECTOR(1, SEGMENT_GDT, 0), %bx
  mov   %bx, %es

  and   $~0x1, %al // Unset PE
  mov   %eax, %cr0

  sti
  pop   %bx
  pop   %eax
  retl

  .global unset_unreal
unset_unreal:
  push  %ax
  xor   %ax, %ax
  mov   %ax, %es
  pop   %ax
  retl

  .section .rodata
unreal_gdtr:
  .word unreal_gdt_end-unreal_gdt-1
  // Offset
  .long unreal_gdt
unreal_gdt:
  .quad 0
  .quad BASE_ADDR(0x100000) | LIMIT_ADDR(0xFFFFF) | ACCESS(DATA_RW) | \
        FLAGS(GRAIN_4K | SIZE_32)
unreal_gdt_end:
