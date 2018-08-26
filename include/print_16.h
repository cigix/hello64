#ifndef PRINT_16
#define PRINT_16

void print_bios_16(const char *str) __attribute__((fastcall));
void print_int_bios_16(long toprint, char size) __attribute__((fastcall));

#endif /* !PRINT_16 */
