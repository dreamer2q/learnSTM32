#include "chess.h"
#include "header.h"
#include <stdio.h>
#include <stdarg.h>

void GAME_printf(const char *fmt, ...) {
  char buf[256] = {0};
  va_list valist;

  va_start(valist, fmt);
  vsnprintf(buf, sizeof buf, fmt, valist);
  va_end(valist);

  POINT_COLOR = BLACK;
  LCD_Fill(0, 304, 240, 320, WHITE);
  LCD_ShowString(0, 304, 240, 16, 12, buf);
};
