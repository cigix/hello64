#include <stddef.h>
#include <stdint.h>

#include "paging.h"

uint32_t *page_directory;
uint32_t *page_table_0;
void *page_0_0;

void setup_paging()
{
  page_directory = (uint32_t*)0x100000;
  page_table_0 = page_directory + 1024; // After the page directory
  page_0_0 = page_table_0 + 1024; // After the page table
  page_directory[0] = PDE_TABLE(WRITABLE | USER, (uint32_t)page_table_0);
  page_table_0[0] = PTE_4KB(WRITABLE | USER, (uint32_t)page_0_0);
  for (size_t i = 1; i < 1024; ++i)
  {
    page_directory[i] = 0;
    page_table_0[i] = 0;
  }
}
