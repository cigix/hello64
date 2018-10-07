#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

// E: Interrupt with error code
// I: IRQ
// X: otherwise
#define INTERRUPT_LIST_EIX \
  X(0x0, Divide by zero) \
  X(0x1, Debug) \
  X(0x2, Non-maskable interrupt) \
  X(0x3, Breakpoint) \
  X(0x4, Overflow) \
  X(0x5, Out of bounds) \
  X(0x6, Invalid opcode) \
  X(0x7, No Coprocessor) \
  E(0x8, Double fault) \
  X(0x9, Coprocessor segment overrun) \
  E(0xa, Bad TSS) \
  E(0xb, Segment not present) \
  E(0xc, Stack fault) \
  E(0xd, General protection fault) \
  E(0xe, Page fault) \
  X(0xf, Unknown interrupt) \
  X(0x10, Coprocessor fault) \
  E(0x11, Alignment check) \
  X(0x12, Machine check) \
  X(0x13, SIMD float exception) \
  X(0x14, Virtualization exception) \
  \
  I(0x20, IRQ0 Clock) \
  I(0x21, IRQ1 Keyboard) \
  I(0x22, IRQ2) \
  I(0x23, IRQ3 Serial port 2) \
  I(0x24, IRQ4 Serial port 1) \
  I(0x25, IRQ5 Sound card) \
  I(0x26, IRQ6 Floppy disk) \
  I(0x27, IRQ7 Parallel port) \
  I(0x28, IRQ8 RTC) \
  I(0x29, IRQ9 ACPI) \
  I(0x2a, IRQ10) \
  I(0x2b, IRQ11) \
  I(0x2c, IRQ12 PS/2) \
  I(0x2d, IRQ13 Coprocessor) \
  I(0x2e, IRQ14 Primary disk) \
  I(0x2f, IRQ15 Secondary disk)

#define IRQ_START (0x20)
#define IDT_SIZE (IRQ_START + 16)

extern uint64_t idt32[IDT_SIZE];
extern uint64_t idt64[IDT_SIZE * 2];

struct interrupt_frame_32
{
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};

struct interrupt_frame_64
{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

# define E(num, name)                                         \
void handler ## num ## _32(struct interrupt_frame_32 *frame,  \
                           uint32_t error_code)               \
  __attribute__((weak));
# define X(num, name)                                         \
void handler ## num ## _32(struct interrupt_frame_32 *frame)  \
  __attribute__((weak));
#define I X
INTERRUPT_LIST_EIX
#undef E
#undef X

# define E(num, name)                                         \
void handler ## num ## _64(struct interrupt_frame_64 *frame,  \
                           uint64_t error_code)               \
  __attribute__((weak));
# define X(num, name)                                         \
void handler ## num ## _64(struct interrupt_frame_64 *frame)  \
  __attribute__((weak));
INTERRUPT_LIST_EIX
#undef E
#undef I
#undef X

#endif /* !INTERRUPTS_H */
