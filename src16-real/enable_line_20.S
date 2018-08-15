  .code16
  .section .text
  .global enable_line_20
enable_line_20:
check_line_20:
  push  %ds
  push  %es
  push  %di
  push  %si

  cli

  xor   %ax, %ax
  mov   %ax, %es
  not   %ax
  mov   %ax, %ds
  mov   $0x0500, %di
  mov   $0x0510, %si

  // es:di = 0000:0500 = 0x000500
  // ds:si = FFFF:0510 = 0x100500
  // If adressing does not use line 20, these two are the same.

  mov   %es:(%di), %al
  push  %ax
  mov   %ds:(%si), %al
  push  %ax

  movb  $0x00, %es:(%di)
  movb  $0xFF, %ds:(%si)

  cmpb  $0xFF, %es:(%di)
  jne   end_restore

  // Fast A20
  in    $0x92, %al
  or    $2, %al
  out   %al, $0x92

end_restore:
  pop   %ax
  mov   %ax, %ds:(%si)
  pop   %ax
  mov   %ax, %es:(%di)
  pop   %si
  pop   %di
  pop   %es
  pop   %ds
  ret
