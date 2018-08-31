#include "descriptor.h"
  .code16
  .section .text.stage0_start
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %es // Setup Extra data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   $0x9C00, %sp // Setup Stack Pointer

  call  enable_line_20

  call  load_stage1

  mov   $text, %cx
  calll print_bios_16

  jmp   to_protected

  .section .rodata
text:
  .asciz "Hello world! (from 16 bits C, BIOS)\r\n\n"
