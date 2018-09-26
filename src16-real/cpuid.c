#include "putc.h"
#include "puts.h"

void print_vendor()
{
  union 
  {
    char str[13];
    struct {
      unsigned ebx, edx, ecx;
    } regs;
  } vendor;
  asm ("cpuid"
       : "=b" (vendor.regs.ebx), "=c" (vendor.regs.ecx), "=d" (vendor.regs.edx)
       : "a" (0));
  vendor.str[12] = '\0';

  puts_serial("CPU vendor: ");
  puts_serial(vendor.str);
  putc_serial('\n');
}
