  .code16
  .section .text
  .global _start
_start:
  xor   %ax, %ax
  mov   %ax, %ds

  mov   $str, %si
  call  proc_print
end:
  jmp end

proc_print:
  lodsb // Load ds:si in al

  and   %al, %al
  jz    proc_print_end // End of string

  mov   $0x0E, %ah // Write character in al
  int   $0x10

  jmp   proc_print

proc_print_end:
  ret

str:
  .asciz "Hello world!"

  .space 510-(.-_start), 0 // Pad with 0s to 510 bytes
  .word 0xAA55 // Boot sector's magic number
