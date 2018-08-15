const char *hexstr = "0123456789ABCDEF";

__attribute__((fastcall)) void print_int_bios_16(long toprint, char size)
{
  short ax;
  char pos;
  char c;
  while(size)
  {
    --size;

    pos = 0xF & (toprint >> (size * 8 + 4));
    c = hexstr[pos];
    ax = 0x0E00 | c;
    asm ("mov %0, %%ax" : : "r" (ax) : "%ax");
    asm ("int $0x10");

    pos = 0xF & (toprint >> (size * 8));
    c = hexstr[pos];
    ax = 0x0E00 | c;
    asm ("mov %0, %%ax" : : "r" (ax) : "%ax");
    asm ("int $0x10");
  }
}
