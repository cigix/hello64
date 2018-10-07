#include "descriptor.h"
  .code64
  .section .text
  .global reload_segments_64
reload_segments_64:
  // Point all other segments to segment index 2, from GDT, with privilege 0
  mov   $SEGMENT_SELECTOR(2, SEGMENT_GDT, 0), %rax
  mov   %rax, %ds // Setup Data Segment
  mov   %rax, %es // Setup Extra data Segment
  mov   %rax, %fs // Setup Extra extra data Segment
  mov   %rax, %gs // Setup Extra extra extra data Segment
  mov   %rax, %ss // Setup Stack Segment
  mov   $0x80000, %rsp
  mov   %rsp, %rbp
  sti

  call  call_win

  hlt
