#ifndef PAGING_H
#define PAGING_H

// Page Directory Entry (page table):
// |     0      |    1     |  2   |       3       |       4       |    5     |
// | Present(1) | Writable | User | Write-through | Cache disable | Accessed |
// |    6    |    7    |8------11|12-----------31|
// | Ignored | Size(0) | Ignored | Address 12:31 |

// Page Directory Entry (4MB page):
// |     0      |    1     |  2   |       3       |       4       |    5     |
// | Present(1) | Writable | User | Write-through | Cache disable | Accessed |
// |   6   |    7    |   8    |9------11| 12  |13------------X|X----------21|
// | Dirty | Size(1) | Global | Ignored | PAT | Address 32:X  | Reserved(0) |
// |22-----------31|
// | Address 22:31 |

// Page Table Entry (4KB page):
// |     0      |    1     |  2   |       3       |       4       |    5     |
// | Present(1) | Writable | User | Write-through | Cache disable | Accessed |
// |    6    |  7  |   8    |9------11|12-----------31|
// | Ignored | PAT | Global | Ignored | Address 12:31 |

#define PDE_TABLE(flags, addr) \
  (((flags) \
    | ((addr) & 0xFFFFF000L) \
    | 1) \
   & ~(1 << 7))
#define PDE_4MB(flags, addr) \
  ((flags) \
   | ( (addr) &    0xFFC00000L) \
   | (((addr) & 0x1FF00000000LL) >> (32 - 13)) \
   | 1 | (1 << 7))
#define PTE_4KB(flags, addr) \
  ((flags) \
   | ((addr) & 0xFFFFF000L)\
   | 1)

#define WRITABLE (1 << 1)
#define USER (1 << 2)
#define WRITETHROUGH (1 << 3)
#define NOCACHE (1 << 4)
#define PAT_4KB (1 << 7)
#define GLOBAL (1 << 8)
#define PAT_4MB (1 << 12)

#endif /* !PAGING_H */
