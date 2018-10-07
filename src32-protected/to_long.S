#include "descriptor.h"
  .code32
  .section .text
  .global to_long
to_long:
  calll setup_paging64
  calll setup_idt64

  cli

  lgdt  gdtr64
  lidt  idtr64

  mov   %cr4, %eax
  or    $(1 << 5), %eax
  mov   %eax, %cr4 // We are now using PAE

  mov   $0xC0000080, %ecx // EFER Model Specific Register
  rdmsr
  or    $(1 << 8), %eax
  wrmsr // We are now in long mode

  mov   %cr0, %eax
  or    $(1 << 31), %eax
  mov   %eax, %cr0 // We are now using paging

  // Reload the code segment to segment index 1, from GDT, with privilege 0
  ljmp  $SEGMENT_SELECTOR(1, SEGMENT_GDT, 0), $reload_segments_64
