#ifndef PUTC_SERIAL_H
#define PUTC_SERIAL_H

#include "io.h"

static inline void putc_serial(const char c)
{
  while (!(inb(LINE_STAT(COM1)) & (1 << 5)))
    continue;
  outb(c, DATA_COM(COM1));
}

void serial_setup();

unsigned puts_serial(const char *str) __attribute__((fastcall));
void puti_serial(long n, char length) __attribute__((fastcall));

#endif /* !PUTC_SERIAL_H */
