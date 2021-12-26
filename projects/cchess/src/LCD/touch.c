#include "stm32f10x.h"
#include "header.h"
#include "lcd.h"
#include <stdio.h>

#define T_CS_SET GPIOA->BSRR = 1 << 15  //片选端口  		PA15
#define T_WR_SET GPIOB->BSRR = 1 << 5   //写数据MOSI			PB5
#define T_SCK_SET GPIOA->BSRR = 1 << 5  // SCK			PA5

#define T_CS_CLR GPIOA->BRR = 1 << 15  //片选端口  		PA15
#define T_WR_CLR GPIOB->BRR = 1 << 5   //写数据	MOSI		PB5
#define T_SCK_CLR GPIOA->BRR = 1 << 5  // SCK			PA5

#define T_IN_STATUE ((GPIOA->IDR & 0X40) ? 1 : 0)  //数据MISO PA6

// 采样次数
#define T_TIMES 4

/**
 * 全局变量
 */
u16 TOUCH_LAST_X, TOUCH_LAST_Y;

/**
 * TOUCH handler 函数指针
 */
// void (*TOUCH_Handler)(event_t* e) = NULL;
void Default_TOUCH_Handler(event_t* e) {
  BEEP_INTERVAL = 3;
  printf("TouchEvent:( type=%u, param1=%u, param2=%u )", e->type, e->param1,
         e->param2);
}

void TOUCH_Configure(void) {
  uint32_t periph_clock = RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                          RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO;
  RCC_APB2PeriphClockCmd(periph_clock, ENABLE);
  // Disable JTAG, Enable SWD
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  /* SPI pins configuration */
  GPIO_InitTypeDef GPIO_InitSPI = {
      .GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_15,
      .GPIO_Mode = GPIO_Mode_Out_PP,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(GPIOA, &GPIO_InitSPI);

  // MOSI
  GPIO_InitSPI.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitSPI);

  // MISO
  GPIO_InitSPI.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitSPI.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitSPI);

  // IT Configure
  GPIO_InitSPI.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitSPI.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitSPI);

  /**
   * TOUCH_INT_EXIT_Init
   */
  EXTI_InitTypeDef EXTI_InitSPI = {
      .EXTI_Line = EXTI_Line2,
      .EXTI_Mode = EXTI_Mode_Interrupt,
      .EXTI_Trigger = EXTI_Trigger_Falling,
      .EXTI_LineCmd = ENABLE,
  };

  /* Connect Button EXTI Line to Button GPIO Pin */
  /* Configure Button EXTI line */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  EXTI_Init(&EXTI_InitSPI);

  EXTI_ClearITPendingBit(EXTI_Line2);

  /**
   * Interrupt
   */
  NVIC_InitTypeDef NVIC_InitPen = {
      .NVIC_IRQChannel = EXTI2_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 1,
      .NVIC_IRQChannelSubPriority = 2,
      .NVIC_IRQChannelCmd = ENABLE,
  };
  /* Set the Vector Table base address at 0x08000000 */
  // TODO 没搞懂为何这样做
  // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

  NVIC_Init(&NVIC_InitPen);

  // if (!TOUCH_Handler) {
  //   TOUCH_Handler = Default_TOUCH_Handler;
  // }
}

u8 SPI_SndRecv(u8 data) {
  u8 tmp = 0, i;

  delay_loop(1);
  for (i = 0; i < 8; i++) {
    T_SCK_CLR;

    if (data & 0x80) {
      T_WR_SET;
    } else {
      T_WR_CLR;
    }
    data = data << 1;
    delay_loop(1);

    T_SCK_SET;
    tmp = tmp << 1;
    tmp |= T_IN_STATUE;
    delay_loop(1);
  }

  return tmp;
}

u32 SPI_X(void) {
  u32 x_addata[T_TIMES] = {0};

  for (u8 i = 0; i < T_TIMES; i++) {  //采样4次.
    T_CS_CLR;                         // cs
    SPI_SndRecv(0xD0);
    x_addata[i] = SPI_SndRecv(0x00);
    x_addata[i] <<= 8;
    x_addata[i] |= SPI_SndRecv(0x00);
    x_addata[i] >>= 3;
    T_CS_SET;
  }

  for (u8 i = 0; i < T_TIMES; i++) {
    for (u8 j = i; j < T_TIMES; j++) {
      if (x_addata[j] > x_addata[i]) {
        u32 tmp = x_addata[i];
        x_addata[i] = x_addata[j];
        x_addata[j] = tmp;
      }
    }
  }

  if (x_addata[1] - x_addata[2] > 25) {
    return 0;
  }

  u32 Temp = (x_addata[1] + x_addata[2]) >> 1;
  u32 r = Temp - 200;
  r *= 240;
  u32 sx = r / (4000 - 200);
  if (sx <= 0 || sx > 240) sx = 0;
  return sx;
}

u32 SPI_Y(void) {
  u32 y_addata[T_TIMES] = {0};
  for (u8 i = 0; i < T_TIMES; i++) {  //采样4次.
    T_CS_CLR;                         // cs
    SPI_SndRecv(0x90);
    y_addata[i] = SPI_SndRecv(0x00);
    y_addata[i] <<= 8;
    y_addata[i] |= SPI_SndRecv(0x00);
    y_addata[i] >>= 3;
    T_CS_SET;
  }
  for (u8 i = 0; i < T_TIMES; i++) {
    for (u8 j = i; j < T_TIMES; j++) {
      if (y_addata[j] > y_addata[i]) {
        u32 k = y_addata[i];
        y_addata[i] = y_addata[j];
        y_addata[j] = k;
      }
    }
  }
  u32 Temp = (y_addata[1] + y_addata[2]) >> 1;
  u32 r = Temp - 190;
  r *= 320;
  u32 sy = r / (4000 - 190);
  if (sy <= 0 || sy > 320) sy = 0;
  return sy;
}

/**
 * TOUCH 中断处理
 */
void EXTI2_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
    // 关闭中断
    EXTI->IMR &= ~EXTI_Line2;

    u8 released = PCin(2);
    u16 tx = SPI_X();
    u16 ty = SPI_Y();
    if (tx) {
      TOUCH_LAST_X = tx;
    }
    if (ty) {
      TOUCH_LAST_Y = ty;
    }

    if (released) {
      CORE_PostEvent(EVENT_Touchup, TOUCH_LAST_X, TOUCH_LAST_Y);
      CORE_PostEvent(EVENT_Touch, TOUCH_LAST_X, TOUCH_LAST_Y);
    } else {
      CORE_PostEvent(EVENT_Touchdown, TOUCH_LAST_X, TOUCH_LAST_Y);
    }

    // 打开中断
    EXTI->IMR |= EXTI_Line2;

    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}
