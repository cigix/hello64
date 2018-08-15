  .code16
  .section .text
  .global al_to_str
al_to_str:
  mov   $hexstr, %si

  mov   %al, %bl
  and   $0x000F, %bx
  mov   (%bx,%si), %bl
  mov   %bl, al_str_buf + 1

  mov   %al, %bl
  and   $0x00F0, %bx
  shr   $4, %bl
  mov   (%bx,%si), %bl
  mov   %bl, al_str_buf

  ret

  .global print_al_bios
print_al_bios:
  call  al_to_str
  mov   $al_str_buf, %si
  call  print_asm_bios
  ret

  .global print_al_vga
print_al_vga:
  call  al_to_str
  mov   $al_str_buf, %si
  call  print_asm_vga
  ret

al_str_buf:
  .asciz  "00"

hexstr:
  .ascii  "0123456789ABCDEF"
