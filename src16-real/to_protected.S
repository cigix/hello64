#include "descriptor.h"
  .code16
  .section .text
  .global to_protected
to_protected:
  // Notify the BIOS that we are going into long mode. We are not yet, but we
  // will likely be there next time we need the BIOS.
  mov   $0xEC00, %ax
  mov   $2, %bl
  int   $0x15

  calll setup_idt

  cli
  lgdt  gdtr
  lidt  idtr
  mov   %cr0, %eax
  or    $0x1, %al
  mov   %eax, %cr0 // We are now in protected mode

  // Reload the code segment to segment index 1, from GDT, with privilege 0
  ljmp  $SEGMENT_SELECTOR(1, SEGMENT_GDT, 0), $reload_segments
