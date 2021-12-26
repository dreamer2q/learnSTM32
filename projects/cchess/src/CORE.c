#include "core.h"
#include "header.h"

/**
 * 全局 core 变量
 */
u32 core_uptime = 0;  // milliseconds
u16 ms_tick = 0;

// 消息事件队列
event_t event_queue[EVENT_QUEUE_SIZE];
event_t *event_head = event_queue;
event_t *event_tail = event_queue;
s8 event_len = 0;

void CORE_PostEvent(enum event_type type, u16 param1, u16 param2) {
  event_head->type = type;
  event_head->param1 = param1;
  event_head->param2 = param2;
  event_head->event_time = core_uptime;
  if (event_len == EVENT_QUEUE_SIZE) {
    // 抛弃一个事件
    CORE_WaitEvent();
  }
  event_head = EVENT_NEXT(event_head);
  event_len++;
}

event_t *CORE_WaitEvent() {
  while (event_len == 0) {
    // wait for new event
  }
  event_len--;
  event_t *e = event_tail;
  event_tail = EVENT_NEXT(event_tail);
  return e;
}

/**
 * core handler
 */
void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    /**
     * 系统时间
     */
    core_uptime += 1;
    ms_tick += 1;
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

/**
 * 系统时钟, tim2
 */
void CORE_Configure() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef TIM_InitCoreTim2 = {
      .TIM_ClockDivision = TIM_CKD_DIV1,
      .TIM_CounterMode = TIM_CounterMode_Down,
      .TIM_Period = 9,
      .TIM_Prescaler = 7199,
  };
  TIM_TimeBaseInit(TIM2, &TIM_InitCoreTim2);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitCoreTim2 = {
      .NVIC_IRQChannel = TIM2_IRQn,
      .NVIC_IRQChannelCmd = ENABLE,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
  };
  NVIC_Init(&NVIC_InitCoreTim2);

  TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief Sets System clock frequency to 72MHz and configure HCLK, PCLK2
 *        and PCLK1 prescalers.
 * @param None.
 * @arg None.
 * @note : This function should be used only after reset.
 * @retval value: None.
 */
void SetSysClockTo72(void) {
  uint32_t StartUpCounter = 0, HSEStatus = 0;

  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /*!< Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /*!< Wait till HSE is ready and if Time out is reached exit */
  do {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while ((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
    HSEStatus = (uint32_t)0x01;
  } else {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01) {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    /*!< PLLCLK = 8MHz * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                         RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
    }
  } else { /*!< If HSE fails to start-up, the application will have wrong clock
                configuration. User can add here some code to deal with this
              error */

    /*!< Go to infinite loop */
    while (1) {
    }
  }
}