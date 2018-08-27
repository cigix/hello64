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

  pushl $0x07 // Light gray
  pushl $12 // y
  pushl $0 // x
  pushl $text
  calll print_vga_32
  add   $16, %esp

end:
  jmp   end

  .section .rodata
text:
  .asciz "Hello world! (from 32 bits C, VGA)"
