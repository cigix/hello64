#include "descriptor.h"
  .code16
  .section .text.bootloader
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %es // Setup Extra data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   $0x9C00, %sp // Setup Stack Pointer

  calll find_extra_data

  lgdt  gdtr

  jmp   to_protected
