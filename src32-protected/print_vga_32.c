short * const vga_text_buffer = (short*)0xB8000;

void print_vga_32(const char *str, char x, char y, char attributes)
{
  while (*str)
  {
    if (!*str)
      return;
    if (*str == '\n')
      ++y;
    else if (*str == '\r')
      x = 0;
    else
    {
      vga_text_buffer[y * 80 + x] = (attributes << 8) | *str;

      ++x;
      if (x >= 80)
      {
        x = 0;
        ++y;
      }
    }
    ++str;
  }
}
