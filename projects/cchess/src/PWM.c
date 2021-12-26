#include <stm32f10x.h>
#include "header.h"

/**
 * PWM CH1N 接入 LED0
 * LED 另需配置
 */
void PWM_Configure(u16 arr, u16 psc) {
  RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  GPIO_InitTypeDef GPIO_InitLED0 = {
      .GPIO_Mode = GPIO_Mode_AF_PP,
      .GPIO_Pin = GPIO_Pin_8,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOE, &GPIO_InitLED0);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseLED0 = {
      .TIM_Period = arr,
      .TIM_Prescaler = psc,
      .TIM_ClockDivision = TIM_CKD_DIV1,
      .TIM_CounterMode = TIM_CounterMode_Up,
  };
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseLED0);

  TIM_OCInitTypeDef TIM_OCInitLED0 = {
      .TIM_OCMode = TIM_OCMode_PWM2,
      .TIM_OutputNState = TIM_OutputState_Enable,
      .TIM_OCNPolarity = TIM_OCPolarity_High,
      .TIM_Pulse = 0,
  };
  TIM_OC1Init(TIM1, &TIM_OCInitLED0);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);

  TIM_Cmd(TIM1, ENABLE);
}
