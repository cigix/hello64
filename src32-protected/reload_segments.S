#include "descriptor.h"
  .code32
  .section .text
  .global reload_segments
reload_segments:
  // Point all other segments to segment index 2, from GDT, with privilege 0
  mov   $SEGMENT_SELECTOR(2, SEGMENT_GDT, 0), %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %es // Setup Extra data Segment
  mov   %ax, %fs // Setup Extra extra data Segment
  mov   %ax, %gs // Setup Extra extra extra data Segment
  mov   %ax, %ss // Setup Stack Segment
  sti

  calll serial_setup

  mov   $text, %ecx
  calll puts_serial

  jmp .

  .section .rodata
text:
  .asciz "Reached protected mode\n"
