#ifndef FUNCTIONS_16_H
#define FUNCTIONS_16_H

#include <stdint.h>

uint16_t read_floppy(uint16_t cylinder, uint8_t head, uint8_t sector,
                     uint8_t count, uint16_t to);

void to_unreal();
void unset_unreal();

#endif /* !FUNCTIONS_16_H */
