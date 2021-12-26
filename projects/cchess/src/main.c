#include <stdio.h>
#include "header.h"
#include "core.h"
#include "lcd.h"

#define SetBITx(x) (1 << (x))
#define ResetBITx(x) (~SetBITx(x))

#define NOP6    \
  __asm("nop"); \
  __asm("nop"); \
  __asm("nop"); \
  __asm("nop"); \
  __asm("nop"); \
  __asm("nop");

// 72MHz
// 1s = 1 000 ms = 1 000 000 us
void delay_us(u16 us) {
  while (us-- > 0) {
    NOP6 NOP6 NOP6 NOP6 NOP6;
    NOP6 NOP6 NOP6 NOP6 NOP6;
  }
}

void delay_ms(u16 ms) {
  while (ms-- > 0) {
    delay_us(1000);
  }
}

void delay_loop(u32 loop) {
  while (loop-- > 0) __asm("nop");  // NOP
}

u8 LED_STATE = 0;
int16_t BEEP_INTERVAL = 0;
// u16 LED0_PWM_VAL = 0;
// u8 LED0_PWM_DIR = 0;
// u16 LCD_COLOR_VALUES[] = {WHITE, BLACK,  BLUE,  RED,  MAGENTA, GREEN,
// CYAN,  YELLOW, BRRED, GRAY, LGRAY,   BROWN};

void Game_Init(void);
void Game_Update(u16 elasped);
void Game_Event(event_t *e);

void mainloop() {
  u32 prev_uptime = 0;
  u32 keycount_start = 0;
  u8 keycount_pressed = 0;
  u8 keycode = 0;
  u16 keycode_state = 0;

  POINT_COLOR = RED;
  LCD_Clear(BLACK);
  LCD_Clear(WHITE);

  Game_Init();

  for (;;) {
    /**
     * Interval
     */
    u16 interval = core_uptime - prev_uptime;
    prev_uptime = core_uptime;

    /**
     * ms TICK
     */
    if (ms_tick >= 1000) {
      // one second
      ms_tick -= 1000;

      // if (LED_STATE & 0x1) {
      //   LED_STATE &= ~0x1;
      // } else {
      //   LED_STATE |= 0x1;
      // }

      // counter++;
      // USART_SendData(USART1, '_');
      // if (core_uptime % 2) {
      // printf("tick~\n");
      // } else {
      // printf("tok~\n");
      // }
      // bgcolor_index = (bgcolor_index + 1) % LEN(LCD_COLORS);
      // LCD_Clear(LCD_COLORS[bgcolor_index]);
    }

    /**
     * LED State
     */
    // PE8,9,10,11
    // LED_PORT->ODR &= ~(0xF << 8);
    // LED_PORT->ODR |= LED_STATE << 8;

    /**
     * PWM
     */
    // if (LED0_PWM_DIR) {
    //   LED0_PWM_VAL++;
    // } else {
    //   LED0_PWM_VAL--;
    // }
    // if (LED0_PWM_VAL == 0) {
    //   LED0_PWM_DIR = 1;
    // } else if (LED0_PWM_VAL > 300) {
    //   LED0_PWM_DIR = 0;
    // }
    // TIM_SetCompare1(TIM1, LED0_PWM_VAL);

    /**
     * KEY BOARD
     */
    keycode = KEY_Scan();
    if (keycode) {
      keycode_state |= SetBITx(keycode - 1);
      CORE_PostEvent(EVENT_Keydown, KEY_BOARD, keycode);
      // DIGIT_Display(keycode, DIGIT_FLAG_DEC);
    } else {
      if (keycode & SetBITx(keycode - 1)) {
        keycode &= ResetBITx(keycode - 1);
        CORE_PostEvent(EVENT_Keyup, KEY_BOARD, keycode);
      }
      // DIGIT_Display(counter, DIGIT_FLAG_DEC);
    }

    /**
     * BEEP
     */
    if (BEEP_INTERVAL > 0) {
      BEEP_On();
      BEEP_INTERVAL -= interval;
    } else {
      BEEP_Off();
    }

    /**
     * COUNTER
     */
    if (COUNT_Scan()) {
      if (keycount_pressed) {
        u16 delta = core_uptime - keycount_start;
        if (1000 < delta && keycount_pressed == 1) {
          // long pressed begin
          CORE_PostEvent(EVENT_KeylongPressed, KEY_COUNTER, 0);
          // long pressed end
          keycount_pressed = 2;
        }
      } else {
        keycount_pressed = 1;
        // keydown
        CORE_PostEvent(EVENT_Keydown, KEY_COUNTER, 0);
        keycount_start = core_uptime;
      }
    } else if (keycount_pressed) {
      keycount_pressed = 0;
      if (core_uptime - keycount_start <= 1000) {
        // pressed begin
        CORE_PostEvent(EVENT_Keypress, KEY_COUNTER, 0);
        // pressed end
      }
      CORE_PostEvent(EVENT_Keyup, KEY_COUNTER, 0);
    }

    /**
     * GAME EVENT
     */
    if (CORE_HasEvent()) {
      Game_Event(CORE_WaitEvent());
    }

    /**
     * GAME LOGIC
     */
    Game_Update(interval);
  }
}

int main(void) {
  SystemInit();
  SetSysClockTo72();
  SystemCoreClockUpdate();

  NVIC_Configure();
  CORE_Configure();
  BEEP_Configure();
  // LED_Configure();
  // PWM_Configure(899, 0);
  // DIGIT_Configure();
  COUNT_Configure();
  KEY_Configure();
  USART_Configure();
  LCD_Configure();
  TOUCH_Configure();
  LCD_Init();

  mainloop();
}
