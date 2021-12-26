#include <stm32f10x.h>

#include "header.h"

void DIGIT_Configure(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_InitTypeDef GPIO_InitCtrl = {
      .GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13,
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOC, &GPIO_InitCtrl);

  GPIO_InitTypeDef GPIO_InitDisplay = {
      .GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7,
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOD, &GPIO_InitDisplay);
}

u8 DIGIT_Encode(u8 num) {
  u8 digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  if (num < LEN(digits)) {
    return digits[num];
  }
  return 0xFF;  // ERROR
}

/**
 * FROM 0x0000 TO 0xFFFF
 */
void DIGIT_Display(u16 number, u8 flag) {
  u8 codec;
  for (u8 i = 0; i < 4; i++) {
    DIGIT_CTRL->ODR |= 0xF << 10;           // turn off all
    DIGIT_CTRL->ODR &= ~(0x1 << (13 - i));  // turn on i_th
    if (flag & DIGIT_FLAG_HEX) {
      codec = DIGIT_Encode(number & 0xF);
      number >>= 4;
    } else {
      codec = DIGIT_Encode(number % 10);
      number /= 10;
    }
    if (flag & (1 << i)) {
      codec |= 0x80;  // show "." dot
    }
    DIGIT_DISP->ODR &= ~(0xFF);  // clear
    DIGIT_DISP->ODR |= ~codec;
    delay_loop(720);
  }
}
