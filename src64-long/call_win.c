#include <stdarg.h>

#include "putc_serial.h"
#include "win.h"

#define FLAG_ALT    (1 << 0)
#define FLAG_ZERO   (1 << 1)
#define FLAG_LEFT   (1 << 2)
#define FLAG_SPACE  (1 << 3)
#define FLAG_PLUS   (1 << 4)
#define FLAG_LOWER  (1 << 5)
#define FLAG_SIGN   (1 << 6)

enum size { CHAR = 1, SHORT = 2, INT = 4, LONG = 8, LONGLONG = 16};

static int printf(const char *fmt, ...)
{
  // This implementation is greatly inspired from:
  // https://github.com/torvalds/linux/blob/master/arch/x86/boot/printf.c
  int count = 0;
  va_list args;
  va_start(args, fmt);

#define PUTC(c) do { putc_serial(c); ++count; } while(0)

  for (; *fmt; ++fmt)
  {
    if (*fmt != '%')
    {
      PUTC(*fmt);
      continue;
    }
    const char *fmt_start = ++fmt;
    if (*fmt == '%')
    {
      PUTC('%');
      continue;
    }

    int done = 0;
    int flags = 0;
    while (!done)
    {
      switch (*fmt)
      {
        case '#':
          flags |= FLAG_ALT;
          break;
        case '0':
          flags |= FLAG_ZERO;
          break;
        case '-':
          flags |= FLAG_LEFT;
          break;
        case ' ':
          flags |= FLAG_SPACE;
          break;
        case '+':
          flags |= FLAG_PLUS;
          break;
        default:
          done = 1;
          continue;
      }
      ++fmt;
    }
    int width = -1;
    if ('0' <= *fmt && *fmt <= '9')
    {
      width = 0;
      while ('0' <= *fmt && *fmt <= '9')
        width = width * 10 + *(fmt++) - '0';
    }
    else if (*fmt == '*')
    {
      ++fmt;
      width = va_arg(args, int);
      if (width < 0)
      {
        width = -width;
        flags |= FLAG_LEFT;
      }
    }
    int precision = -1;
    if (*fmt == '.')
    {
      ++fmt;
      if ('0' <= *fmt && *fmt <= '9')
      {
        precision = 0;
        while ('0' <= *fmt && *fmt <= '9')
          precision = precision * 10 + *(fmt++) - '0';
      }
      else if (*fmt == '*')
      {
        ++fmt;
        precision = va_arg(args, int);
      }
      if (precision < 0)
        precision = 0;
    }

    enum size arg_length = INT;
    if (*fmt == 'h')
    {
      arg_length = SHORT;
      ++fmt;
      if (*fmt == 'h')
      {
        arg_length = CHAR;
        ++fmt;
      }
    }
    if (*fmt == 'l' || *fmt == 'L')
    {
      arg_length = LONG;
      ++fmt;
      if (*fmt == 'l' || *fmt == 'L')
      {
        arg_length = LONGLONG;
        ++fmt;
      }
    }

    int base = 10;
    switch (*fmt)
    {
      case 'c':
        if (!(flags & FLAG_LEFT))
        {
          while (0 < --width)
            PUTC(' ');
        }
        PUTC(va_arg(args, int));
        while (0 < --width)
          PUTC(' ');
        continue;
      case 's':
        {
          char *s = va_arg(args, char *);

          int s_len = 0;
          while (s[s_len])
          {
            ++s_len;
            if (0 <= precision && precision <= s_len)
              break;
          }

          if (!(flags & FLAG_LEFT))
          {
            while (s_len < width--)
              PUTC(' ');
          }
          for (int i = 0; i < s_len; ++i)
            PUTC(s[i]);
          while (s_len < width--)
            PUTC(' ');
        }
        continue;
      case 'p':
        if (width == -1)
        {
          width = 2 * sizeof(void *);
          flags |= FLAG_ZERO;
        }
        base = 16;
        arg_length = sizeof(void *);
        break;
      case 'n':
        switch (arg_length)
        {
          case CHAR:
            {
              char *p = va_arg(args, char *);
              *p = count;
            }
            break;
          case SHORT:
            {
              short *p = va_arg(args, short *);
              *p = count;
            }
            break;
          case LONG:
            {
              long *p = va_arg(args, long *);
              *p = count;
            }
            break;
          case LONGLONG:
            {
              long long *p = va_arg(args, long long *);
              *p = count;
            }
            break;
          case INT:
          default:
            {
              int *p = va_arg(args, int *);
              *p = count;
            }
            break;
        }
        continue;
      case 'o':
        base = 8;
        break;
      case 'x':
        flags |= FLAG_LOWER;
        __attribute__((fallthrough));
      case 'X':
        base = 8;
        break;
      case 'd':
      case 'i':
        flags |= FLAG_SIGN;
      case 'u':
        break;
      default:
        PUTC('%');
        fmt = fmt_start;
        continue;
    }
    long long num;
    switch (arg_length)
    {
      case CHAR:
        if (flags & FLAG_SIGN)
          num = (char)va_arg(args, int);
        else
          num = (unsigned char)va_arg(args, unsigned int);
        break;
      case SHORT:
        if (flags & FLAG_SIGN)
          num = (short)va_arg(args, int);
        else
          num = (unsigned short)va_arg(args, unsigned int);
        break;
      case INT:
        if (flags & FLAG_SIGN)
          num = (int)va_arg(args, int);
        else
          num = (unsigned int)va_arg(args, unsigned int);
        break;
      case LONG:
        if (flags & FLAG_SIGN)
          num = (long)va_arg(args, long);
        else
          num = (unsigned long)va_arg(args, unsigned long);
        break;
      case LONGLONG:
        if (flags & FLAG_SIGN)
          num = (long long)va_arg(args, long long);
        else
          num = (unsigned long long)va_arg(args, unsigned long long);
        break;
    }

    char digits[16] = "0123456789ABCDEF";
    char buf[66];

    if (flags & FLAG_LOWER)
    {
      for (unsigned i = 10; i < sizeof(digits); ++i)
        digits[i] += 'a' - 'A';
    }
    if (flags & FLAG_LEFT)
      flags &= ~FLAG_ZERO;
    char pad = ' ';
    if (flags & FLAG_ZERO)
      pad = '0';
    char sign = 0;
    if (flags & FLAG_SIGN)
    {
      if (num < 0)
      {
        sign = '-';
        num = -num;
        --width;
      }
      else if (flags & FLAG_PLUS)
      {
        sign = '+';
        --width;
      }
      else if (flags & FLAG_SPACE)
      {
        sign = ' ';
        --width;
      }
    }
    if (flags & FLAG_ALT)
      width -= base >> 3; // -1 for base 8, -2 for base 16

    int i = 0;
    if (num)
    {
      while(num)
      {
        buf[i++] = digits[num % base];
        num /= base;
      }
    }
    else
      buf[i++] = 0;
    if (i > precision)
      precision = i;
    width -= precision;
    if (!(flags & (FLAG_ZERO | FLAG_LEFT)))
    {
      while (0 < width--)
        PUTC(' ');
    }
    if (sign)
      PUTC(sign);
    if (flags & FLAG_ALT)
    {
      if (8 <= base)
        PUTC('0');
      if (base == 16)
      {
        if (flags & FLAG_LOWER)
          PUTC('x');
        else
          PUTC('X');
      }
    }
    if (!(flags & FLAG_LEFT))
    {
      while (0 < width--)
        PUTC(pad);
    }
    while (i < precision--)
      PUTC('0');
    while (0 < i--)
      PUTC(buf[i]);
    while (0 < width--)
      PUTC(' ');
  }

  va_end(args);
  return count;
}

void call_win()
{
  serial_setup();
  printf("Successfully reached long mode\r\n");
  printf("Calling win(printf)...\r\n\n");
  win(printf);
  printf("\r\nwin() exited, halting");
}
