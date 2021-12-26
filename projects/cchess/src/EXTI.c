#include <stm32f10x.h>

#include "header.h"

u8 led_status = 0;

void EXTI9_5_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
    /**
     * Counter Handler
     */
    if (led_status) {
      LED1_OFF;
    } else {
      LED1_ON;
    }
    led_status = !led_status;

    EXTI_ClearITPendingBit(EXTI_Line6);
  }
}
