#ifndef PUTI_H
#define PUTI_H

#include "putc.h"

static inline void puti_generic(const unsigned long n, char length,
                                const putc_function putc)
{
  const char *hexstr = "0123456789ABCDEF";
  while (length)
  {
    --length;
    int pos = 0xF & (n >> (length * 4));
    putc(hexstr[pos]);
  }
}

#endif /* !PUTI_H */
