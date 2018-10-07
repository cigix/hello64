#include <stdint.h>

#include "interrupts.h"
#include "putc_serial.h"

#define E(num, name)                                          \
__attribute__((interrupt,weak))                               \
void handler ## num ## _64(struct interrupt_frame_64 *frame,  \
                           uint64_t error_code)               \
{                                                             \
  puts_serial("Interrupt: ");                                 \
  puts_serial(#name);                                         \
  puts_serial(" (");                                          \
  puts_serial(#num);                                          \
  puts_serial(", 64) at 0x");                                 \
  puti_serial(frame->ip, 16);                                 \
  puts_serial(" with error code ");                           \
  puti_serial(error_code, 16);                                \
  putc_serial('\n');                                          \
  for(;;);                                                    \
}
#define I(num, name)                                          \
__attribute__((interrupt,weak))                               \
void handler ## num ## _64(struct interrupt_frame_64 *frame)  \
{                                                             \
  (void)frame;                                                \
  puts_serial("Interrupt: ");                                 \
  puts_serial(#name);                                         \
  puts_serial(" (64)\n");                                     \
}
#define X(num, name)                                          \
__attribute__((interrupt,weak))                               \
void handler ## num ## _64(struct interrupt_frame_64 *frame)  \
{                                                             \
  puts_serial("Interrupt: ");                                 \
  puts_serial(#name);                                         \
  puts_serial(" (");                                          \
  puts_serial(#num);                                          \
  puts_serial(", 64) at 0x");                                 \
  puti_serial(frame->ip, 16);                                 \
  putc_serial('\n');                                          \
  for(;;);                                                    \
}
INTERRUPT_LIST_EIX
#undef E
#undef I
#undef X

uint64_t idt64[IDT_SIZE * 2] = {0};

struct
{
  uint16_t size;
  uint64_t offset;
} idtr64 = { sizeof (idt64) - 1, (uint64_t)&idt64 };
