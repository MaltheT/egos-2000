/*
 * (C) 2026, Cornell University
 * All rights reserved.
 *
 * Description: formatted printing
 * format_to_str() converts a format into a C string:
 * e.g., converts ("%s-%d", "egos", 2000) to "egos-2000".
 * term_write() prints the converted C string to the screen.
 */

#include "egos.h"
#include "servers.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void format_to_str(char *out, const char *fmt, va_list args) {
  for (out[0] = 0; *fmt != '\0'; fmt++) {
    if (*fmt != '%') {
      strncat(out, fmt, 1);
    } else {
      fmt++;
      if (*fmt == 's') {
        strcat(out, va_arg(args, char *));
      } else if (*fmt == 'd') {
        itoa(va_arg(args, int), out + strlen(out), 10);
      } else if (*fmt == 'x') {
        itoa(va_arg(args, int), out + strlen(out), 16);
      }
      /* Student's code goes here (Hello, World!). */

      /* Handle format %c, %u, %p, %lld, %llu and %llx. */
      else if (*fmt == 'c') {
        char c = (char)va_arg(args, int);
        char tmp[2] = {c, '\0'};
        strcat(out, tmp);
      } else if (*fmt == 'u') {
        uint val = va_arg(args, uint);
        char tmp[12];
        int i = 0;
        if (val == 0)
          tmp[i++] = '0';
        else {
          char rev[12];
          int j = 0;
          while (val > 0) {
            rev[j++] = '0' + (val % 10);
            val /= 10;
          }
          while (j > 0)
            tmp[i++] = rev[--j];
        }
        tmp[i] = '\0';
        strcat(out, tmp);
      } else if (*fmt == 'p') {
        uint val = va_arg(args, uint);
        strcat(out, "0x");
        itoa(val, out + strlen(out), 16);
      } else if (*fmt == 'l' && *(fmt + 1) == 'l') {
        fmt += 2;
        if (*fmt == 'd') {
          long long val = va_arg(args, long long);
          char tmp[22];
          int i = 0;
          int neg = 0;
          if (val < 0) {
            neg = 1;
            val = -val;
          }
          if (val == 0)
            tmp[i++] = '0';
          else {
            char rev[22];
            int j = 0;
            while (val > 0) {
              rev[j++] = '0' + (val % 10);
              val /= 10;
            }
            while (j > 0)
              tmp[i++] = rev[--j];
          }
          tmp[i] = '\0';
          if (neg) {
            char tmp2[22] = "-";
            strcat(tmp2, tmp);
            strcat(out, tmp2);
          } else
            strcat(out, tmp);
        } else if (*fmt == 'u') {
          unsigned long long val = va_arg(args, unsigned long long);
          char tmp[22];
          int i = 0;
          if (val == 0)
            tmp[i++] = '0';
          else {
            char rev[22];
            int j = 0;
            while (val > 0) {
              rev[j++] = '0' + (val % 10);
              val /= 10;
            }
            while (j > 0)
              tmp[i++] = rev[--j];
          }
          tmp[i] = '\0';
          strcat(out, tmp);
        } else if (*fmt == 'x') {
          unsigned long long val = va_arg(args, unsigned long long);
          char tmp[18];
          int i = 0;
          if (val == 0)
            tmp[i++] = '0';
          else {
            char rev[18];
            int j = 0;
            const char *hex = "0123456789abcdef";
            while (val > 0) {
              rev[j++] = hex[val & 0xf];
              val >>= 4;
            }
            while (j > 0)
              tmp[i++] = rev[--j];
          }
          tmp[i] = '\0';
          strcat(out, tmp);
        }
      } else if (*fmt == 'h' && *(fmt + 1) == 'h' && *(fmt + 2) == 'u') {
        fmt += 2;
        uint val = (uchar)va_arg(args, int);
        char tmp[4];
        int i = 0;
        if (val == 0)
          tmp[i++] = '0';
        else {
          char rev[4];
          int j = 0;
          while (val > 0) {
            rev[j++] = '0' + (val % 10);
            val /= 10;
          }
          while (j > 0)
            tmp[i++] = rev[--j];
        }
        tmp[i] = '\0';
        strcat(out, tmp);
      }

      /* Student's code ends here. */
    }
  }
}

#define LOG(prefix, suffix)                                                    \
  char buf[512];                                                               \
  strcpy(buf, prefix);                                                         \
  va_list args;                                                                \
  va_start(args, format);                                                      \
  format_to_str(buf + strlen(prefix), format, args);                           \
  va_end(args);                                                                \
  strcat(buf, suffix);                                                         \
  term_write(buf, strlen(buf));

int my_printf(const char *format, ...) { LOG("", ""); }

int INFO(const char *format, ...) { LOG("[INFO] ", "\n\r") }

int FATAL(const char *format, ...) {
  LOG("\x1B[1;31m[FATAL] ", "\x1B[1;0m\n\r") /* \x1B[1;31m means red. */
  while (1)
    ;
}

int SUCCESS(const char *format, ...) {
  LOG("\x1B[1;32m[SUCCESS] ", "\x1B[1;0m\n\r") /* \x1B[1;32m means green. */
}

int CRITICAL(const char *format, ...) {
  LOG("\x1B[1;33m[CRITICAL] ", "\x1B[1;0m\n\r") /* \x1B[1;33m means yellow. */
}
