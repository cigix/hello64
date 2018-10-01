#include "descriptor.h"
  .code16
  .section .text.stage0_start
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %es // Setup Extra data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   $0xF000, %sp // Setup Stack Pointer

  calll load_stage1

  call  enable_line_20

  calll load_stage2

  mov   $text, %cx
  calll puts_bios

  jmp   to_protected

  .section .rodata
text:
  .asciz "Jumping into protected mode...\r\n"
