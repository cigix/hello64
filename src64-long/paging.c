#include <stdint.h>

#include "interrupts.h"
#include "paging.h"
#include "putc_serial.h"

__attribute__((interrupt))
void handler0xe_64(struct interrupt_frame_64 *frame, uint64_t error_code)
{
  puts_serial("Interrupt: Page fault (0xe) at 0x");
  puti_serial(frame->ip, 16);
  puts_serial(" trying to access 0x");
  uint64_t cr2;
  asm ("mov %%cr2, %0" : "=r" (cr2));
  puti_serial(cr2, 16);
  puts_serial(" with error code ");
  puti_serial(error_code, 16);
  putc_serial('\r');
  putc_serial('\n');
}
