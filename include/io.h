#ifndef IO_H
#define IO_H

char inb(short port) __attribute__((fastcall));
void outb(char value, short port) __attribute__((fastcall));

#define COM1 0x3f8

#define DATA(port)          (port)
#define DIVISOR_LOW(port)   (port)
#define DIVISOR_HIGH(port)  (port + 1)
#define INTERRUPT(port)     (port + 1)
#define INT_FIFO_CTL(port)  (port + 2)
#define LINE_CTL(port)      (port + 3)
#define MODEM_CTL(port)     (port + 4)
#define LINE_STAT(port)     (port + 5)
#define MODEM_STAT(port)    (port + 6)
#define SCRATCH(port)       (port + 7)

#define DATA_LENGTH(length) ((length - 5) & 0x3)
#define STOP_LENGTH(length) (((length - 1) & 0x1) << 2)
#define PARITY(par)         ((par & 0x7) << 3)
#define PARITY_NONE         PARITY(0x0)
#define PARITY_ODD          PARITY(0x1)
#define PARITY_EVEN         PARITY(0x3)
#define PARITY_MARK         PARITY(0x5)
#define PARITY_SPACE        PARITY(0x7)
#define DLAB                (1 << 7)

#define FIFO_ENABLE         (1)
#define FIFO_CLEAR_RECEIVE  (1 << 1)
#define FIFO_CLEAR_SEND     (1 << 2)
#define INT_TRIGGER(level)  ((level & 0x3) << 6)

#endif /* !IO_H */
