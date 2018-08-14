  .code16
  .section .text
  .global print_asm_bios
print_asm_bios:
print_asm_bios_loop:
  lodsb // Load *(ds:si) in al, increment si

  // Check for end of string
  and   %al, %al
  jz    print_asm_bios_end

  mov   $0x0E, %ah // Write the character stored in al

  int   $0x10

  jmp   print_asm_bios_loop

print_asm_bios_end:
  ret
