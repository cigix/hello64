#include "descriptor.h"
  .code64
  .section .text
  .global reload_segments_64
reload_segments_64:
  // Point all other segments to segment index 2, from GDT, with privilege 0
  mov   $SEGMENT_SELECTOR(2, SEGMENT_GDT, 0), %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %es // Setup Extra data Segment
  mov   %ax, %fs // Setup Extra extra data Segment
  mov   %ax, %gs // Setup Extra extra extra data Segment
  mov   %ax, %ss // Setup Stack Segment
  sti

  mov   $puts_serial, %ecx
  call  win
  hlt
