  .code16
  .section .text
  .global load_next_sector
load_next_sector:
  mov   $2, %ah // Read sectors
  mov   $1, %al // # of sectors to read
  mov   $0, %ch // Cylinder
  mov   $2, %cl // Sector
  mov   $0, %dh // Head
  mov   $0, %dl // Drive : Floppy 1
  mov   $0x7E00, %bx
  int   $0x13
  jc    error
  test  %ah, %ah
  jnz   error
  ret

error:
  mov   $error_txt, %cx
  calll print_bios_16
  jmp   .

  .section .rodata
error_txt:
  .asciz "Error loading next sector"
