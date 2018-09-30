#include "interrupts.h"
#include "putc_serial.h"

__attribute__((interrupt))
void handler0xe(struct interrupt_frame *frame, uint32_t error_code)
{
  puts_serial("Interrupt: Page fault (0xe) at 0x");
  puti_serial(frame->ip, 8);
  puts_serial(" trying to access 0x");
  uint32_t cr2;
  asm ("mov %%cr2, %%eax" : "=a" (cr2));
  puti_serial(cr2, 8);
  puts_serial(" with error code ");
  puti_serial(error_code, 8);
  putc_serial('\n');
}
