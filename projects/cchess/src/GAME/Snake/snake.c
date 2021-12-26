#include "header.h"
#include "lcd.h"
#include "core.h"
#include "string.h"
#include "stdio.h"

#define WIDTH 20
#define HEIGHT 20
#define S_SIZE (WIDTH * HEIGHT)

#define GET_X(i) (i % WIDTH)
#define GET_Y(i) (i / HEIGHT)
#define MOVE_NEXT(node) (body + (node - body + 1) % S_SIZE)

enum snake {
  Road = 0,
  Wall,
  Body,
  Food,
};

enum snake_dir {
  S_NONE = 0,
  S_UP = 5,
  S_LEFT = 9,
  S_RIGHT = 11,
  S_DOWN = 15,
};

/// 全局变量

/**
 * 贪吃蛇地图
 */
u8 snake_map[WIDTH][HEIGHT];

/**
 * 贪吃蛇身体
 */
struct snode {
  u8 x;
  u8 y;
} body[S_SIZE], *header, *tail;

/**
 * 状态
 */
u16 s_score = 0;
u16 s_speed = 500;
u16 s_tick = 0;
u8 s_dir = S_NONE;

void draw_info() {
  POINT_COLOR = WHITE;
  char infos[128];
  sprintf(infos, "Score: %-3u Speed: %-3u", s_score, s_speed);
  LCD_ShowString(5, 265 + 5, 240, 12, 12, infos);
}

void draw_node(u16 x, u16 y, u8 type) {
  const u8 scale = 12;
  x *= scale;
  y *= scale;
  u16 x2 = x + scale, y2 = y + scale;
  switch (type) {
    case Road:
      LCD_Fill(x, y, x2, y2, WHITE);
      break;
    case Wall:
      LCD_Fill(x, y, x2, y2, GREEN);
      break;
    case Body:
      POINT_COLOR = BLUE;
      LCD_Fill(x, y, x2, y2, WHITE);
      LCD_Fill(x + scale / 3, y + scale / 3, x2 - scale / 3, y2 - scale / 3,
               BLUE);
      LCD_Draw_Circle(x + scale / 2, y + scale / 2, scale / 2 - 2);
      LCD_Draw_Circle(x + scale / 2, y + scale / 2, scale / 2 - 3);
      break;
    case Food:
      POINT_COLOR = RED;
      LCD_Fill(x, y, x2, y2, WHITE);
      LCD_Draw_Circle(x + scale / 2, y + scale / 2, scale / 2 - 2);
      break;
  }
}

void draw_update(void) {
  for (u8 x = 0; x < WIDTH; x++) {
    for (u8 y = 0; y < HEIGHT; y++) {
      draw_node(x, y, snake_map[x][y]);
    }
  }
  POINT_COLOR = WHITE;
  LCD_DrawRectangle(12, 12, 240 - 12, 240 - 12);

  BACK_COLOR = BLACK;
  POINT_COLOR = WHITE;
  LCD_ShowString(0, 241, 240, 24, 24, "Snake Game by JL");

  draw_info();
}

void make_food(void) {
  u16 fd = rand() % S_SIZE;
  u8 x = GET_X(fd), y = GET_Y(fd);
  while (snake_map[x][y] != Road) {
    fd = rand() % S_SIZE;
    x = GET_X(fd);
    y = GET_Y(fd);
  }
  snake_map[x][y] = Food;

  draw_node(x, y, Food);
}

void snake_die(void) {
  POINT_COLOR = RED;
  LCD_ShowString(20, 100, 100, 20, 24, "you died!");
}

void Snake_Init(void) {
  srand(core_uptime);
  memset(snake_map, 0, sizeof(snake_map));
  memset(body, 0, sizeof(body));
  header = body + 2;
  tail = body;

  for (u8 x = 0; x < WIDTH; x++) {
    for (u8 y = 0; y < HEIGHT; y++) {
      if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) {
        snake_map[x][y] = Wall;
      }
    }
  }

  for (u8 i = 0; i < 3; i++) {
    u8 x = WIDTH / 3 + i;
    u8 y = HEIGHT / 2;
    body[i].x = x;
    body[i].y = y;
    snake_map[x][y] = Body;
  }

  draw_update();
  make_food();
}

void Snake_Update(u16 elasped) {
  s_tick += elasped;
  if (s_tick >= s_speed) {
    s_tick -= s_speed;
    // move next
    u8 nx = header->x, ny = header->y;
    switch (s_dir) {
      case S_UP:
        ny--;
        break;

      case S_LEFT:
        nx--;
        break;

      case S_RIGHT:
        nx++;
        break;

      case S_DOWN:
        ny++;
        break;

      default:
        // do nothing
        return;
    }

    u8 is_food = snake_map[nx][ny] == Food;
    switch (snake_map[nx][ny]) {
      case Road:
      case Food:
        // move on
        header = MOVE_NEXT(header);
        snake_map[nx][ny] = Body;
        header->x = nx;
        header->y = ny;
        draw_node(nx, ny, Body);
        if (is_food) {
          make_food();
          s_score += 10;
          s_speed -= 10;
          draw_info();
        } else {
          snake_map[tail->x][tail->y] = Road;
          draw_node(tail->x, tail->y, Road);
          tail = MOVE_NEXT(tail);
        }
        break;
      case Wall:
      case Body:
        // die
        s_dir = S_NONE;
        snake_die();
        break;
    }
  }
}

void Snake_Event(event_t* e) {
  BEEP_INTERVAL = 3;
  printf("type=%u, %u, %u\n", e->type, e->param1, e->param2);

  u8 dir = S_NONE;
  u16 x = e->param1;
  u16 y = e->param2 * 240 / 320;
  if (e->type == EVENT_Touchdown) {
    if (x < y) {
      if (x < 60 && y < 240 - x) {
        dir = S_LEFT;
      } else {
        dir = S_DOWN;
      }
    } else {
      if (x > 60 && y > 240 - x) {
        dir = S_RIGHT;
      } else {
        dir = S_UP;
      }
    }
    if (dir != S_NONE) {
      if (dir + s_dir != 20) {
        s_dir = dir;
      }
    }
    LCD_ShowxNum(6 + 12 * 5, 264 + 16, s_dir, 2, 12, 0);
  }
}
