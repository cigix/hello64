#include "win.h"

__attribute__((weak))
void win(printf_ptr printf)
{
        printf("Hello World (from weak win)\n");
}
