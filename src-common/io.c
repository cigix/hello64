#include "io.h"

__attribute__((fastcall)) char inb(short port)
{
  char value;
  asm ("in %%dx, %%al" : "=a" (value) : "d" (port));
  return value;
}

__attribute__((fastcall)) void outb(char value, short port)
{
  asm ("out %%al, %%dx" : : "a" (value), "d" (port));
}
