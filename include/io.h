#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
  uint8_t value;
  asm volatile ("in %1, %0" : "=a" (value) : "Nd" (port));
  return value;
}

static inline void outb(char value, short port)
{
  asm volatile ("out %0, %1" : : "a" (value), "Nd" (port));
}

#define COM1 0x3f8

#define DATA_COM(port)      (port)
#define DIVISOR_LOW(port)   (port)
#define DIVISOR_HIGH(port)  ((port) + 1)
#define INTERRUPT(port)     ((port) + 1)
#define INT_FIFO_CTL(port)  ((port) + 2)
#define LINE_CTL(port)      ((port) + 3)
#define MODEM_CTL(port)     ((port) + 4)
#define LINE_STAT(port)     ((port) + 5)
#define MODEM_STAT(port)    ((port) + 6)
#define SCRATCH(port)       ((port) + 7)

#define DATA_LENGTH(length) (((length) - 5) & 0x3)
#define STOP_LENGTH(length) ((((length) - 1) & 0x1) << 2)
#define PARITY(par)         (((par) & 0x7) << 3)
#define PARITY_NONE         PARITY(0x0)
#define PARITY_ODD          PARITY(0x1)
#define PARITY_EVEN         PARITY(0x3)
#define PARITY_MARK         PARITY(0x5)
#define PARITY_SPACE        PARITY(0x7)
#define DLAB                (1 << 7)

#define FIFO_ENABLE         (1)
#define FIFO_CLEAR_RECEIVE  (1 << 1)
#define FIFO_CLEAR_SEND     (1 << 2)
#define INT_TRIGGER(level)  (((level) & 0x3) << 6)

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0

#define COMMAND(port)  (port)
#define DATA_PIC(port) ((port) + 1)

// Initialisation Command Words
#define ICW1_ICW4     (1 << 0) // Require ICW4 word
#define ICW1_SINGLE   (1 << 1) // Single PIC (otherwise cascaded)
#define ICW1_INTER4   (1 << 2) // 4 bytes between ISR (otherwise 8)
#define ICW1_TRIGLVL  (1 << 3) // Level-triggered (otherwise edge-triggered)
#define ICW1_INIT     (1 << 4) // Required

#define ICW2_OFFSET(offset) (offset)

#define ICW3_MASTER_SLAVES(slaves) (slaves)
#define ICW3_SLAVE_ID(id) ((id) & 0x7)

#define ICW4_8086   (1 << 0) // Work in 8086 mode (otherwise 8085)
#define ICW4_AEOI   (1 << 1) // Automatic end of interrupt
#define ICW4_MASTER (1 << 2) // Set as master
#define ICW4_BUF    (1 << 3) // Buffered mode
#define ICW4_SFNM   (1 << 4) // Special fully nested mode

#endif /* !IO_H */
