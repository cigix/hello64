// Global Descriptor:
// |0----8----15|16-24-32-39|40----47|48---------51+52---55|56--------63|
// | Limit 0:15 | Base 0:23 | Access | Limit 16:19 | Flags | Base 24:31 |

#define BASE_ADDR(addr) \
  (((addr & 0x00FFFFFFL) << 16) | ((addr & 0xFF000000L) << (56 - 24)))

#define LIMIT_ADDR(addr) \
  ((addr & 0x0FFFFL) | ((addr & 0xF0000L) << (48 - 16)))

#define ACCESS(byte) ((byte & 0xFE | 0x80LL) << 40)
#define FLAGS(flags) ((flags & 0xDLL) << 52)

// Access:
// |      0      |       1       |          2           |
// | Accessed(0) | Read/Writable | Direction/Conforming |
// |     3      |   4    |5---------6|     7      |
// | Executable | System | Privilege | Present(1) |

#define PRIVILEGE(priv) (priv << 5)
#define SYSTEM (1 << 4)
#define DATA (0)
#define EXECUTABLE (1 << 3)
#define EXPAND_UP (0)
#define EXPAND_DOWN (1 << 2)
#define NONCONFORMING (0)
#define CONFORMING (1 << 2)
#define READ_EXEC (1 << 1)
#define WRITE_DATA (1 << 1)

#define EXEC_XO (EXECUTABLE)
#define EXEC_XR (EXECUTABLE | READ_EXEC)
#define DATA_RO (DATA)
#define DATA_RW (DATA | WRITE_DATA)

// Flags:
// |     0     |      1      |  2   |      3      |
// | Available | Reserved(0) | Size | Granularity |

#define GRAIN_1B (0)
#define GRAIN_4K (1 << 3)
#define SIZE_16 (0)
#define SIZE_32 (1 << 2)
#define AVAIL_1 (1)

// Interrupt Descriptor:
// |0----8-----15|16----24-----31|32---------39|40--47|48----56----63|
// | Offset 0:15 | Selector 0:15 | Reserved(0) | Type | Offset 16:31 |

#define OFFSET_ADDR(addr) \
  ((addr & 0x0000FFFF) | ((addr & 0xFFFF0000) << (48 - 16)))

#define SELECTOR_ADDR(addr) (addr << 16)

#define TYPE(type) ((type | 0x80L) << 40)

// Type:
// |0----3|    4    |5---------6|     7      |
// | Type | Storage | Privilege | Present(1) |

#define TYPE_

#define SEGMENT_SELECTOR(index, table, privilege) \
  (index << 3 | table << 2 | privilege)
#define SEGMENT_GDT (0)
#define SEGMENT_LDT (1)
