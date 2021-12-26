#include <stm32f10x.h>
#include "header.h"
#include <stdio.h>

char USART_STDIN_BUF[USART_BUFSIZ];
char *USART_BUFPTR = USART_STDIN_BUF;
char *USART_BUFTAIL = USART_STDIN_BUF;
u16 USART_BUFLEN = 0;

#define MOVE_NEXT(ptr) __NEXT_PTR_LOOP(USART_STDIN_BUF, ptr, USART_BUFSIZ)

inline int USART_haschar(void) { return USART_BUFLEN > 0; }

int USART_getline(char *buf) {
  char ch;
  while ((ch = USART_getchar()) != '\n') {
    *buf++ = ch;
  }
  *buf = '\n';
  return 0;
}

int USART_getchar(void) {
  while (USART_BUFLEN == 0) {
    // wait ...
  }
  char ch = *USART_BUFTAIL;
  USART_BUFLEN--;
  USART_BUFTAIL = MOVE_NEXT(USART_BUFTAIL);
  return ch;
}

int USART_putchar(uint16_t ch) {
  while (!(USART1->SR & USART_FLAG_TC)) {
    // 等待数据发送完成
  }
  USART1->DR = ch;
  return ch;
}

void USART1_IRQHandler(void) {
  if (USART1->SR & USART_FLAG_RXNE) {
    // 对USART_DR的读操作可以将该位清零。
    u8 ch = USART1->DR;
    *USART_BUFPTR = ch;
    if (USART_BUFLEN == USART_BUFSIZ) {
      // 缓冲区溢出, 抛弃一个数据
      USART_BUFLEN--;
      USART_BUFTAIL = MOVE_NEXT(USART_BUFTAIL);
    }
    USART_BUFPTR = MOVE_NEXT(USART_BUFPTR);
    USART_BUFLEN++;
  }
  // if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
  //   u16 res = USART_ReceiveData(USART1);
  //   USART_SendData(USART1, res);
  //   USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  // }
}

void USART_Configure(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

  USART_DeInit(USART1);

  /*
   *  USART1_TX -> PA9 , USART1_RX ->	PA10
   */
  GPIO_InitTypeDef GPIO_UsartTX = {
      .GPIO_Pin = GPIO_Pin_9,
      .GPIO_Mode = GPIO_Mode_AF_PP,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };

  GPIO_InitTypeDef GPIO_UsartRX = {
      .GPIO_Pin = GPIO_Pin_10,
      .GPIO_Mode = GPIO_Mode_IN_FLOATING,
      .GPIO_Speed = GPIO_Speed_50MHz,
  };

  GPIO_Init(GPIOA, &GPIO_UsartRX);
  GPIO_Init(GPIOA, &GPIO_UsartTX);

  /**
   * 配置 USART 参数
   */
  USART_InitTypeDef USART_InitStruct = {
      .USART_BaudRate = 9600,
      .USART_WordLength = USART_WordLength_8b,
      .USART_StopBits = USART_StopBits_1,
      .USART_Parity = USART_Parity_No,
      .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
      .USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
  };
  USART_Init(USART1, &USART_InitStruct);

  /**
   * 配置中断
   */
  NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART1_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 3,
      .NVIC_IRQChannelSubPriority = 3,
      .NVIC_IRQChannelCmd = ENABLE,
  };
  NVIC_Init(&NVIC_InitStruct);

  /**
   * 开启中断
   */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
  USART_ClearITPendingBit(USART1, USART_IT_TC);
}
