#ifndef PUTC_SERIAL_H
#define PUTC_SERIAL_H

#include "fastcall.h"
#include "io.h"

static inline void putc_serial(const char c)
{
  while (!(inb(LINE_STAT(COM1)) & (1 << 5)))
    continue;
  outb(c, DATA_COM(COM1));
}

void serial_setup();

unsigned puts_serial(const char *str) FASTCALL;
void puti_serial(long n, char length) FASTCALL;

#endif /* !PUTC_SERIAL_H */
