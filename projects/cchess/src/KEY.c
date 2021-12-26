#include <stm32f10x.h>
#include "header.h"

#define KEY_IN_MASK 0x0F
#define KEY_OUT_MASK 0xF0

void KEY_Configure(void) {
  // GPOIE 用于键盘扫描
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  // GPIOE->CRL = 0x33338888;  // PE.0,1,2,3 上拉输入; PE.4,5,6,7 推挽输出
  GPIO_InitTypeDef GPIO_InitKeyIN = {
      .GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3,
      .GPIO_Mode = GPIO_Mode_IPU,  // 上拉输入
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOE, &GPIO_InitKeyIN);

  GPIO_InitTypeDef GPIO_InitKeyOUT = {
      .GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7,
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOE, &GPIO_InitKeyOUT);
}

u8 KEY_Scan(void) {
  GPIOE->ODR &= ~KEY_OUT_MASK;  // clear first
  u8 keydown = ~(GPIOE->IDR & KEY_IN_MASK) & KEY_IN_MASK;
  if (keydown) {  // 低电平代表有按键按下
    for (u8 row = 0; row < 4; row++) {
      GPIOE->ODR |= (GPIO_Pin_4 << row);
      delay_loop(10);
      if (GPIOE->IDR & keydown) {
        for (u8 col = 0; col < 4; col++) {
          if (keydown & (1 << col)) {
            return row * 4 + col + 1;
          }
        }
      }
      GPIOE->ODR &= ~(GPIO_Pin_4 << row);
    }
  }
  return 0;
}
