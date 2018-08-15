  .code16
  .section .text
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds // Setup Data Segment
  mov   %ax, %ss // Setup Stack Segment
  mov   $0xB800, %ax // Text video memory
  mov   %ax, %es // Setup Extra data Segment
  mov   $0x9C00, %sp // Setup Stack Pointer

  mov   $test, %cx
  call  print_C_bios

  cli

end:
  jmp end

  .section .rodata
test:
  .asciz  "test\r\n"
