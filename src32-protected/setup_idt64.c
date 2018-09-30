#include "io.h"
#include "interrupts.h"
#include "descriptor.h"

void setup_idt64()
{
#define X(num, name) \
  idt64[num * 2] = OFFSET_ADDR((uint64_t)handler ## num ## _64) | \
    SELECTOR_ADDR(SEGMENT_SELECTOR(1, SEGMENT_GDT, 0)) | \
    TYPE(GATE(TRAP_64) | PRIVILEGE(0)); \
  idt64[num * 2 + 1] = ((uint64_t)handler ## num ## _64) >> 32;
#define E X
#define I X
INTERRUPT_LIST_EIX
#undef E
#undef I
#undef X
}
