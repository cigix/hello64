#include <stdint.h>

#include "paging.h"
#include "putc_serial.h"

#define KiB * 1024
#define MiB KiB KiB

extern char ram_start, stack_start, stage2_start, stage2_end;

static uint64_t max_addr;

void* get_4K_page()
{
  static unsigned count = 0;
  max_addr += 4 KiB;
  return &ram_start + 4 KiB * (count++);
}

void create_mapping_4K(uint64_t virtual, uint64_t physical, unsigned flags)
{
  uint64_t *pml4t, *pdpt, *pdt, *pt;
  unsigned i_pt =     (virtual >> 12) & 0x1FF;
  unsigned i_pdt =    (virtual >> 21) & 0x1FF;
  unsigned i_pdpt =   (virtual >> 30) & 0x1FF;
  unsigned i_pml4t =  (virtual >> 39) & 0x1FF;

  asm volatile ("mov %%cr3, %0" : "=r" (pml4t));
  pml4t = (void*)ADDR_12_51((uint32_t)pml4t);

  if (pml4t[i_pml4t])
    pdpt = (uint64_t*)(ADDR_12_51(pml4t[i_pml4t]));
  else
  {
    pdpt = get_4K_page();
    for (int i = 0; i < 512; ++i)
      pdpt[i] = 0;
    pml4t[i_pml4t] = PML4E((uint32_t)pdpt);
  }

  if (pdpt[i_pdpt])
    pdt = (uint64_t*)(ADDR_12_51(pdpt[i_pdpt]));
  else
  {
    pdt = get_4K_page();
    for (int i = 0; i < 512; ++i)
      pdt[i] = 0;
    pdpt[i_pdpt] = PDPE((uint32_t)pdt);
  }

  if (pdt[i_pdt])
    pt = (uint64_t*)(ADDR_12_51(pdt[i_pdt]));
  else
  {
    pt = get_4K_page();
    for (int i = 0; i < 512; ++i)
      pt[i] = 0;
    pdt[i_pdpt] = PDE((uint32_t)pt);
  }

  pt[i_pt] = PTE(physical) | flags;
}

void create_mapping_2M(uint64_t virtual, uint64_t physical, unsigned flags)
{
  uint64_t *pml4t, *pdpt, *pdt;
  unsigned i_pdt =    (virtual >> 21) & 0x1FF;
  unsigned i_pdpt =   (virtual >> 30) & 0x1FF;
  unsigned i_pml4t =  (virtual >> 39) & 0x1FF;

  asm volatile ("mov %%cr3, %0" : "=r" (pml4t));
  pml4t = (uint64_t*)ADDR_12_51((uint32_t)pml4t);

  if (pml4t[i_pml4t])
    pdpt = (uint64_t*)(ADDR_12_51(pml4t[i_pml4t]));
  else
  {
    pdpt = get_4K_page();
    for (int i = 0; i < 512; ++i)
      pdpt[i] = 0;
    pml4t[i_pml4t] = PML4E((uint32_t)pdpt);
  }

  if (pdpt[i_pdpt])
    pdt = (uint64_t*)(ADDR_12_51(pdpt[i_pdpt]));
  else
  {
    pdt = get_4K_page();
    for (int i = 0; i < 512; ++i)
      pdt[i] = 0;
    pdpt[i_pdpt] = PDPE((uint32_t)pdt);
  }

  pdt[i_pdt] = PDE_4MB(physical) | flags;
}

void setup_paging64()
{
  max_addr = &ram_start;

  // Put the first tables in lower memory
  asm ("mov %0, %%cr3" : : "r" (CR3_64(0, 0x1000)));
  uint64_t *pml4t = (uint64_t*)0x1000;
  uint64_t *pdpt = (uint64_t*)0x2000;
  uint64_t *pdt = (uint64_t*)0x3000;
  uint64_t *pt = (uint64_t*)0x4000;
  for (int i = 1; i < 512; ++i)
  {
    pml4t[i] = 0;
    pdpt[i] = 0;
    pdt[i] = 0;
    pt[i] = 0;
  }
  // Map first MiB flat, read only, system
  pml4t[0] = PML4E(0x2000);
  pdpt[0] = PDPE(0x3000);
  pdt[0] = PDE(0x4000);
  for (int i = 0; i < 256; ++i)
    pt[i] = PTE(4 KiB * i);

  // Map stage 2 flat, read write, user
  for (uint64_t addr = (uintptr_t)&stage2_start; addr < (uintptr_t)&stage2_end; )
  {
    if (addr + 2 MiB <= (uintptr_t)&stage2_end)
    {
      create_mapping_2M(addr, addr, WRITABLE | USER);
      addr += 2 MiB;
    }
    else
    {
      create_mapping_4K(addr, addr, WRITABLE | USER);
      addr += 4 KiB;
    }
  }

  // Map stack flat, read write, user
  create_mapping_2M((uintptr_t)&stack_start, (uintptr_t)&stack_start,
                    WRITABLE | USER);

  // Map page tables flat, read only, user
  for (uint64_t addr = (uintptr_t)&ram_start; addr < max_addr; addr += 2 MiB)
    create_mapping_2M(addr, addr, 0);
}
