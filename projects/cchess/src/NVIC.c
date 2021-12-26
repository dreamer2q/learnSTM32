#include <stm32f10x.h>
#include <stm32f10x_exti.h>

void NVIC_Configure(void) {
  /**
   *  2 bits for pre-emption priority
   *  2 bits for subpriority
   */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
