#include <stdint.h>

#include "interrupts.h"
#include "putc_serial.h"

#define E(num, name)                                          \
__attribute__((interrupt,weak))                               \
void handler ## num ## _32(struct interrupt_frame_32 *frame,  \
                           uint32_t error_code)               \
{                                                             \
  puts_serial("Interrupt: ");                                 \
  puts_serial(#name);                                         \
  puts_serial(" (");                                          \
  puts_serial(#num);                                          \
  puts_serial(") at 0x");                                     \
  puti_serial(frame->ip, 8);                                  \
  puts_serial(" with error code ");                           \
  puti_serial(error_code, 8);                                 \
  putc_serial('\n');                                          \
}
#define I(num, name)                                         \
__attribute__((interrupt,weak))                              \
void handler ## num ## _32(struct interrupt_frame_32 *frame) \
{                                                            \
  (void)frame;                                               \
  puts_serial("Interrupt: ");                                \
  puts_serial(#name);                                        \
  putc_serial('\n');                                         \
}
#define X(num, name)                                         \
__attribute__((interrupt,weak))                              \
void handler ## num ## _32(struct interrupt_frame_32 *frame) \
{                                                            \
  puts_serial("Interrupt: ");                                \
  puts_serial(#name);                                        \
  puts_serial(" (");                                         \
  puts_serial(#num);                                         \
  puts_serial(") at 0x");                                    \
  puti_serial(frame->ip, 8);                                 \
  putc_serial('\n');                                         \
}
INTERRUPT_LIST_EIX
#undef E
#undef I
#undef X

uint64_t idt32[IDT_SIZE] = {0};

struct
{
  uint16_t size;
  uint32_t offset;
} idtr32 = { sizeof (idt32) - 1, (uint32_t)&idt32 };
