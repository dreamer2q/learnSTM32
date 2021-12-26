#include <stm32f10x.h>

void BEEP_Configure(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct = {
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Pin = GPIO_Pin_3,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void BEEP_On(void) { GPIO_SetBits(GPIOB, GPIO_Pin_3); }
void BEEP_Off(void) { GPIO_ResetBits(GPIOB, GPIO_Pin_3); }
