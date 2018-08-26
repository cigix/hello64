#include "descriptor.h"
  .code16
  .section .text
  .global to_protected
to_protected:
  cli
  mov   %cr0, %eax
  or    $0x1, %al
  mov   %eax, %cr0 // We are now in protected mode

  // Reload the code segment to segment index 1, from GDT, with privilege 0
  ljmp  $SEGMENT_SELECTOR(1, SEGMENT_GDT, 0), $reload_segments
