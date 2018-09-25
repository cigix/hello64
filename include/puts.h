#ifndef PUTS_H
#define PUTS_H

unsigned puts_bios(const char *str) __attribute__((fastcall));
unsigned puts_vga(const char *str) __attribute__((fastcall));
unsigned puts_serial(const char *str) __attribute__((fastcall));

#endif /* !PUTS_H */
