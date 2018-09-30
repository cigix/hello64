#include "putc_serial.h"

#include "puti.h"
#include "puts.h"

void serial_setup()
{
  // Disable all interrupts
  outb(0, INTERRUPT(COM1));
  // Set DLAB
  outb(DLAB, LINE_CTL(COM1));
  // Set divisor
  outb(1, DIVISOR_LOW(COM1));
  outb(0, DIVISOR_HIGH(COM1));
  // Reset DLAB, 8 bits, 1 stop bit, no parity
  outb((~DLAB) & (DATA_LENGTH(8) | STOP_LENGTH(1) | PARITY_NONE),
       LINE_CTL(COM1));
  // Enable FIFO, clear buffers, set largest buffer trigger size
  outb(FIFO_ENABLE | FIFO_CLEAR_RECEIVE | FIFO_CLEAR_SEND | INT_TRIGGER(3),
       INT_FIFO_CTL(COM1));
}

__attribute__((fastcall)) void puti_serial(long n, char length)
{
  return puti_generic(n, length, putc_serial);
}

__attribute__((fastcall)) unsigned puts_serial(const char *str)
{
  return puts_generic(str, putc_serial);
}
