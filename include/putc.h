#ifndef PUTC_H
#define PUTC_H

__attribute__((fastcall)) void vga_set_xy(const char x, const char y);
__attribute__((fastcall)) void vga_set_attributes(const char attributes);

typedef void (*putc_function)(const char c);

void putc_bios(const char c);
void putc_vga(const char c);
void putc_serial(const char c);

#endif /* !PUTC_H */
