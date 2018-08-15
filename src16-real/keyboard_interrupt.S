  .code16
  .section .text
  .global key_interrupt
key_interrupt:
  in    $0x60, %al
  mov   %al, key
  mov   %al, %bl
  
  in    $0x61, %al
  mov   %al, %ah
  or    $0x80, %al
  out   %al, $0x61
  xchg  %al, %ah
  out   %al, $0x61

  mov   $0x20, %al
  out   %al, $0x20

  and   $0x80, %bl
  jnz   key_interrupt_end

  mov   $key_interrupt_text_before, %si
  call  print_asm_bios
  mov   key, %al
  call  print_al_bios
  mov   $key_interrupt_text_after, %si
  call  print_asm_bios

key_interrupt_end:
  iret

key:
  .byte 0

key_interrupt_text_before:
  .asciz "Key pressed: "
key_interrupt_text_after:
  .asciz "\r\n"
