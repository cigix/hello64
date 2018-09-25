#ifndef PUTI_H
#define PUTI_H

void puti_bios(long n, char length) __attribute__((fastcall));
void puti_vga(long n, char length) __attribute__((fastcall));
void puti_serial(long n, char length) __attribute__((fastcall));

#endif /* !PUTI_H */
