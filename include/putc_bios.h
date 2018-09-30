#ifndef PUTC_BIOS_H
#define PUTC_BIOS_H

#include "fastcall.h"

static inline void putc_bios(const char c)
{
  asm volatile ("int $0x10" : : "a" (0x0E00 | c));
}

unsigned puts_bios(const char *str) FASTCALL;
void puti_bios(long n, char length) FASTCALL;

#endif /* !PUTC_BIOS_H */
