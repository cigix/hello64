__attribute__((weak))
void win(int (*printf_ptr)(const char *fmt, ...))
{
        printf_ptr("Hello World (from weak win)\n");
}
