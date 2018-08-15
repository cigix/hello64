  .code16
  .section .text
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   %ax, %fs // Setup Extra extra data Segment
  mov   %ax, %gs // Setup Extra extra extra data Segment
  mov   $0xB800, %ax // Text video memory
  mov   %ax, %es // Setup Extra data Segment
  mov   $0x9C00, %sp // Setup Stack Pointer

  cli
  mov   $0x09*4, %bx
  movw  $key_interrupt, %gs:(%bx)
  mov   %ds, %gs:2(%bx)
  sti

end:
  jmp end

#include "print_asm_bios.S"
#include "print_asm_vga.S"
#include "al_to_str.S"
#include "keyboard_interrupt.S"

  .space 510-(.-_start), 0 // Pad with 0s to 510 bytes
  .word 0xAA55 // Boot sector's magic number
