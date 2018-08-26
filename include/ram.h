#ifndef RAM_H
#define RAM_H

#include <stdint.h>

struct e820_entry
{
  uint32_t base_lo;
  uint32_t base_hi;
  uint32_t length_lo;
  uint32_t length_hi;
  uint32_t type;
#if ACPI3
  uint32_t attributes;
#endif
};

#endif /* !RAM_H */
