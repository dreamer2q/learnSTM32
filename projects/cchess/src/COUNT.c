#include <stm32f10x.h>
#include "header.h"

void COUNT_Configure() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitTypeDef GPIO_InitCounter = {
      .GPIO_Mode = GPIO_Mode_IPU,
      .GPIO_Pin = GPIO_Pin_6,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOB, &GPIO_InitCounter);
}

void COUNT_SetupEXTI(void) {
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);

  EXTI_InitTypeDef EXTI_InitCounter = {
      .EXTI_Line = EXTI_Line6,
      .EXTI_Mode = EXTI_Mode_Interrupt,
      .EXTI_Trigger = EXTI_Trigger_Falling,
      .EXTI_LineCmd = ENABLE,
  };
  EXTI_Init(&EXTI_InitCounter);

  NVIC_InitTypeDef NVIC_InitCounter = {
      .NVIC_IRQChannel = EXTI9_5_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 2,
      .NVIC_IRQChannelSubPriority = 3,
      .NVIC_IRQChannelCmd = ENABLE,
  };
  NVIC_Init(&NVIC_InitCounter);
}

u8 COUNT_Scan(void) {
  if (GPIOB->IDR & GPIO_Pin_6) {
    return 0;
  }
  return 1;
}
