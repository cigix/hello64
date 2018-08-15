OUTPUT_FORMAT("elf32-i386");
ENTRY(_start);

SECTIONS
{
  . = 0x7C00;

  .text : SUBALIGN(0) {
    *(.text.bootloader);
    *(.text*)
  }

  .rodata : SUBALIGN(0) {
    *(.rodata)
  }

  .data : SUBALIGN(0) {
    *(.data)
  }

  .sig : AT(0x7DFE)
  {
    SHORT(0xAA55)
  }

  /DISCARD/ : {
    *(.eh_frame);
    *(.comment);
    *(.note*);
  }
}
