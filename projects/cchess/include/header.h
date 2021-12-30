#ifndef __HEADER_H
#define __HEADER_H

#include "stm32f10x.h"

/**
 * Extend built type
 */

typedef char BOOL;
#define TRUE ((BOOL)1)
#define FALSE (!TRUE)

/**
 * Event Struct
 */
#define __NEXT_PTR_LOOP(head, ptr, size) (head + (ptr - head + 1) % size)

enum key_type {
  KEY_COUNTER,
  KEY_BOARD,
};

enum event_type {
  EVENT_unkown,
  EVENT_Keydown,
  EVENT_Keyup,
  EVENT_Keypress,
  EVENT_KeylongPressed,
  EVENT_Touchdown,
  EVENT_Touchup,
  EVENT_Touch,
};

typedef struct _event_t {
  u8 type;
  u16 param1;
  u16 param2;
  u32 event_time;
} event_t;

#define EVENT_QUEUE_SIZE 10
#define EVENT_NEXT(event) __NEXT_PTR_LOOP(event_queue, event, EVENT_QUEUE_SIZE)

extern event_t event_queue[EVENT_QUEUE_SIZE];
extern event_t *event_head, *event_tail;
extern s8 event_len;

void CORE_PostEvent(enum event_type type, u16 param1, u16 param2);
event_t *CORE_WaitEvent();
inline BOOL CORE_HasEvent(void) { return event_len > 0; }

/**
 * macro utils
 */
#define LEN(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

/**
 * common utils
 */
void delay_loop(u32 loop);
void delay_ms(u16 ms);
void opt_delay(u8 i);
void delay_us(u16 us);

/**
 * Beep is OK
 */
extern int16_t BEEP_INTERVAL;
void BEEP_Configure(void);
void BEEP_On(void);
void BEEP_Off(void);

/**
 * LED is OK
 */
void LED_Configure(void);
#define LED_PORT GPIOE
#define LED1_ON GPIO_ResetBits(GPIOE, GPIO_Pin_8)
#define LED2_ON GPIO_ResetBits(GPIOE, GPIO_Pin_9)
#define LED3_ON GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define LED4_ON GPIO_ResetBits(GPIOE, GPIO_Pin_11)
#define LED1_OFF GPIO_SetBits(GPIOE, GPIO_Pin_8)
#define LED2_OFF GPIO_SetBits(GPIOE, GPIO_Pin_9)
#define LED3_OFF GPIO_SetBits(GPIOE, GPIO_Pin_10)
#define LED4_OFF GPIO_SetBits(GPIOE, GPIO_Pin_11)

/**
 * DIGIT is OK
 */
void DIGIT_Configure(void);
#define DIGIT_CTRL GPIOC
#define DIGIT_DISP GPIOD
#define DIGIT_FLAG_DEC (0 << 7)
#define DIGIT_FLAG_HEX (1 << 7)
#define DIGIT_FLAG_Dot0 (1 << 3)
#define DIGIT_FLAG_Dot1 (1 << 2)
#define DIGIT_FLAG_Dot2 (1 << 1)
#define DIGIT_FLAG_DOT3 (1 << 0)
void DIGIT_Display(u16 number, u8 flag);

/**
 * NVIC is OK
 */
void NVIC_Configure(void);

/**
 * COUNT is OK
 */
void COUNT_Configure(void);
void COUNT_SetupEXTI(void);
u8 COUNT_Scan(void);

/**
 * KEY is OK
 */
void KEY_Configure(void);
u8 KEY_Scan(void);

/**
 * USART is OK
 */
#define USART_BUFSIZ 2048
// extern char USART_STDIN_BUF[USART_BUFSIZ];
// extern char *USART_BUFPTR, *USART_BUFTAIL;
// extern u16 USART_BUFLEN;
void USART_Configure(void);
int USART_haschar(void);
int USART_putchar(uint16_t ch);
int USART_getchar(void);
int USART_getline(char *buf);
int USART_readline(char *buf, int len);

/**
 * PWM is ?
 */
void PWM_Configure(u16 arr, u16 psc);

#endif
