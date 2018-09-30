#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

// Common fields:
// |     0      |    1     |  2   |       3       |       4       |    5     |
// | Present(1) | Writable | User | Write-through | Cache disable | Accessed |
// |32-----40-----48---51|52----56-------62|        63        |
// | Address 32:51 (x64) | Available (x64) | No Execute (x64) |

// CR3:
// |0-----------2|3------4|5-----8----11|12--16---24--31|32-----40-----48---51|
// | Reserved(0) | Common | Reserved(0) | Address 12:31 | Address 32:51 (x64) |
// |52---------63|
// | Reserved(0) |

// Page Map Level 4 Entry:
// |0------5|    6    |7-----------8|9--------11|12--16--24---31|32----63|
// | Common | Ignored | Reserved(0) | Available | Address 12:31 | Common |

// Page Directory Pointer Entry:
// |0------5|    6    |    7    |    8    |9--------11|12-----------31|32----63|
// | Common | Ignored | Size(0) | Ignored | Available | Address 12:31 | Common |

// Page Directory Pointer Entry (1GB page):
// |0------5|   6   |    7    |   8    |9--------11| 12  |13---------30|
// | Common | Dirty | Size(1) | Global | Available | PAT | Reserved(0) |
// |30-----------31|32----63|
// | Address 30:31 | Common |

// Page Directory Entry:
// |0------5|    6    |    7    |    8    |9--------11|12-----------31|32----63|
// | Common | Ignored | Size(0) | Ignored | Available | Address 12:31 | Common |

// Page Directory Entry (2MB page, X64):
// |0------5|   6   |    7    |   8    |9--------11| 12  |13---------20|
// | Common | Dirty | Size(1) | Global | Available | PAT | Reserved(0) |
// |21-----------31|32----63|
// | Address 21:31 | Common |

// Page Directory Entry (4MB page, x32):
// |0------5|   6   |    7    |   8    |9--------11| 12  |13-----------20|
// | Common | Dirty | Size(1) | Global | Available | PAT | Address 32:39 |
// |     21      |22-----------31|
// | Reserved(0) | Address 22:31 |

// Page Table Entry (4KB page):
// |0------5|   6   |  7  |   8    |9--------11|12-----------31|32----63|
// | Common | Dirty | PAT | Global | Available | Address 12:31 | Common |

#define WRITABLE (1 << 1)
#define USER (1 << 2)
#define WRITETHROUGH (1 << 3)
#define NOCACHE (1 << 4)
#define GLOBAL (1 << 8)
#define NOEXEC (1 << 63)

#define PAT_4KB (1 << 7)
#define PAT_4MB (1 << 12)

#define ADDR_12_31(addr) ((addr) &      0xFFFFF000UL)
#define ADDR_12_51(addr) ((addr) & 0xFFFFFFFFFF000ULL)
#define ADDR_21_51(addr) ((addr) & 0xFFFFFFFE00000ULL)
#define ADDR_22_31(addr) ((addr) &      0xFFC00000UL)
#define ADDR_30_51(addr) ((addr) & 0xFFFFFC0000000ULL)
#define ADDR_32_39(addr) ((addr) &    0xFF00000000ULL)

#define CR3_32(flags, addr) (((flags) & 0xC) | ADDR_12_31(addr))
#define CR3_64(flags, addr) (((flags) & 0xC) | ADDR_12_51(addr))

#define PML4E(addr)     (0x01 | ADDR_12_51(addr))
#define PDPE(addr)      (0x01 | ADDR_12_51(addr))
#define PDPE_1GB(addr)  (0x41 | ADDR_30_51(addr))
#define PDE(addr)       (0x01 | ADDR_12_51(addr))
#define PDE_2MB(addr)   (0x41 | ADDR_21_51(addr))
#define PDE_4MB(addr)   (0x41 | ADDR_22_31(addr) | (ADDR_32_39(addr) >> 19))
#define PTE(addr)       (0x01 | ADDR_12_51(addr))

#endif /* !PAGING_H */
