#ifndef PUTC_H
#define PUTC_H

void vga_set_xy(const char x, const char y) __attribute__((fastcall));
void vga_set_attributes(const char attributes) __attribute__((fastcall));
void serial_setup();

typedef void (*putc_function)(const char c) __attribute__((fastcall));

void putc_bios(const char c) __attribute__((fastcall));
void putc_vga(const char c) __attribute__((fastcall));
void putc_serial(const char c) __attribute__((fastcall));

#endif /* !PUTC_H */
