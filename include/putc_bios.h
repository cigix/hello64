#ifndef PUTC_BIOS_H
#define PUTC_BIOS_H

static inline void putc_bios(const char c)
{
  asm ("int $0x10" : : "a" (0x0E00 | c));
}

unsigned puts_bios(const char *str) __attribute__((fastcall));
void puti_bios(long n, char length) __attribute__((fastcall));

#endif /* !PUTC_BIOS_H */
