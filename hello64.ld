OUTPUT_FORMAT("elf32-i386");
ENTRY(_start);

SECTIONS
{
  /* Code taken from the default GNU ld 2.31.1 linker script. */
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF Extension.  */
  .debug_macro    0 : { *(.debug_macro) }
  .debug_addr     0 : { *(.debug_addr) }
  /* End of taken code. */

  /DISCARD/ : {
    *(.eh_frame);
    *(.comment);
    *(.note*);
  }

  .text.stage0 0x7C00 : SUBALIGN(0) {
    stage0.elf(.text.stage0_start)
    stage0.elf(.text*)
  }

  .data.stage0 : SUBALIGN(0) {
    stage0.elf(.rodata* .data*)
  }

  .sig 0x7DFE :
  {
    SHORT(0xAA55)
  }

  .text.stage1 0x7E00 : SUBALIGN(0) {
    stage1_start = .;
    stage1.elf(.text*)
  }

  .data.stage1 : SUBALIGN(0) {
    stage1.elf(.rodata* .data*)
    . = ALIGN(0x200);
    stage1_end = .;
  }

  .stage2 : SUBALIGN(0) {
    stage2_start = .;
    *(*)
    stage2_end = .;
  }
}
