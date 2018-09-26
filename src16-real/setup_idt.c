#include "io.h"
#include "interrupts.h"
#include "descriptor.h"

void setup_idt()
{
#define X(num, name) \
  idt[num] = OFFSET_ADDR((uint32_t)handler ## num) | \
    SELECTOR_ADDR(SEGMENT_SELECTOR(1, SEGMENT_GDT, 0)) | \
    TYPE(GATE(TRAP_32) | PRIVILEGE(0));
#define E X
#define I X
INTERRUPT_LIST_EIX
#undef E
#undef I
#undef X
  // Remap PIC IRQs
  char mask_master_pic = inb(DATA_PIC(PIC_MASTER));
  char mask_slave_pic = inb(DATA_PIC(PIC_SLAVE));

  outb(ICW1_ICW4 | ICW1_INIT, COMMAND(PIC_MASTER));
  outb(ICW1_ICW4 | ICW1_INIT, COMMAND(PIC_SLAVE));
  outb(ICW2_OFFSET(IRQ_START), DATA_PIC(PIC_MASTER));
  outb(ICW2_OFFSET(IRQ_START + 8), DATA_PIC(PIC_SLAVE));
  outb(ICW3_MASTER_SLAVES(1 << 2), DATA_PIC(PIC_MASTER)); // Slave at IRQ2
  outb(ICW3_SLAVE_ID(2), DATA_PIC(PIC_SLAVE)); // Slave at IRQ2
  outb(ICW4_8086 | ICW4_AEOI, DATA_PIC(PIC_MASTER));
  outb(ICW4_8086 | ICW4_AEOI, DATA_PIC(PIC_SLAVE));

  outb(mask_master_pic | 1, DATA_PIC(PIC_MASTER)); // Mask out IRQ0
  outb(mask_slave_pic, DATA_PIC(PIC_SLAVE));
}
