  .code16
  .section .text
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   $0x9C00, %sp // Setup Stack Pointer
  cld
  mov   $0xB800, %ax // Text video memory
  mov   %ax, %es // Setup Extra data Segment

  mov   $str_asm_vga, %si
  call  print_asm_vga

  mov   $str_asm_bios, %si
  call  print_asm_bios

end:
  jmp end


str_asm_bios:
  .asciz "assembly/BIOS\r\n"
str_asm_vga:
  .asciz "assembly/VGA\r\n"

#include "print_asm_bios.S"
#include "print_asm_vga.S"
#include "al_to_str.S"

  .space 510-(.-_start), 0 // Pad with 0s to 510 bytes
  .word 0xAA55 // Boot sector's magic number
