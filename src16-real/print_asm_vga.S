  .code16
  .section .text
  .global print_asm_vga
print_asm_vga:
print_asm_vga_loop:
  lodsb // Load *(ds:si) in al, increment si

  // Check if...
  cmp   $'\n', %al // Line feed
  je    print_asm_vga_lf
  cmp   $'\r', %al // Carriage return
  je    print_asm_vga_cr
  and   %al, %al // End of string
  jz    print_asm_vga_end

  mov   $0x0F, %ah // White on black
  mov   %ax, %cx

  xor   %ax, %ax
  mov   xpos, %al
  shl   %ax
  mov   %ax, %di
  xor   %ax, %ax
  mov   ypos, %al
  mov   $160, %bx
  mul   %bx
  add   %ax, %di

  mov   %cx, %ax
  stosw

  incb  xpos

  jmp   print_asm_vga_loop

print_asm_vga_lf:
  incb  ypos
  jmp   print_asm_vga_loop
print_asm_vga_cr:
  movb  $0, xpos
  jmp   print_asm_vga_loop

print_asm_vga_end:
  ret

xpos:
  .byte  0
ypos:
  .byte  0
