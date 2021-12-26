#include <stm32f10x.h>

void LED_Configure(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct = {
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };

  GPIO_Init(GPIOE, &GPIO_InitStruct);
}
