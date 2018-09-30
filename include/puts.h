#ifndef PUTS_H
#define PUTS_H

#include "putc.h"

static inline unsigned puts_generic(const char *str, const putc_function putc)
{
  unsigned count = 0;
  while (str[count])
    putc(str[count++]);
  return count;
}

#endif /* !PUTS_H */
