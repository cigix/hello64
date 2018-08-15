__attribute__((fastcall)) void print_bios_16(char *str)
{
  short ax;
  while (*str)
  {
    ax = 0x0E00 + *str;
    asm ("mov %0, %%ax" : : "r" (ax) : "%ax");
    asm ("int $0x10");
    
    ++str;
  }
}
