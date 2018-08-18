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

  call  enable_line_20

  mov   $text, %cx
  calll print_bios_16

  mov   $test_string, %cx
  calll print_bios_16

  cli
  lgdt  gdtr
  mov   %cr0, %eax
  or    $0x1, %al
  mov   %eax, %cr0 // We are now in protected mode

  // Reloads the code segment to segment index 1, from GDT, with privilege 0
  ljmp  $SEGMENT_SELECTOR(1, SEGMENT_GDT, 0), $reload_segments

  .section .rodata
text:
  .asciz "Hello world! (from 16 bits C, BIOS)\r\n\n"
