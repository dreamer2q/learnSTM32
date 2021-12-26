#include <errno.h>
#include <sys/unistd.h>  // STDOUT_FILENO, STDERR_FILENO
#include "stm32f10x_usart.h"
#include "lcd.h"
#include "header.h"

int _write(int file, char *ptr, int len) {
  for (int i = 0; i < len; i++) {
    USART_putchar(*ptr++);
  }
  return len;
}

int _read(int file, char *ptr, int len) {
  for (int i = 0; i < len; i++) {
    if (USART_haschar()) {
      *ptr++ = USART_getchar();
    } else {
      return i;
    }
  }
  return len;
}
