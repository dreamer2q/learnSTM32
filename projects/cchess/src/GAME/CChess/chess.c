#include "stm32f10x.h"
#include "header.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "chess.h"

#define __DEBUG
#ifdef __DEBUG
#define debug(block) (block);
#else
#define debug(block)
#endif
#define log_debug(fmt, ...) debug({ printf(fmt, __VA_ARGS__); })

// 棋盘 Y 偏移量
#define OFFY 26
#define EV_MAX

#define HAS_PIECE(sq) (chess_board[(sq)] != 0)

/**
 * 全局变量
 */

// 对局模式
enum gm {
  PLAYER_VS_PLAYER,
  PLAYER_VS_COMPUTER,
};
enum gm CC_MODE = PLAYER_VS_COMPUTER;

enum cchess {
  NOT_BEGIN,
  GAME_PAUSED,
  GAME_PLAYING,
  GAME_WIN,
  GAME_DRAW,
};
enum cchess CC_STATE = NOT_BEGIN;

void ev_click_chess(event_t *);
void ev_click_welcome(event_t *);
void ev_undo_chess(void);

// 事件区域
struct ev_area_t {
  u16 x, y;
  u16 w, h;
  void (*handler)(event_t *);
  enum cchess filter;
} EV_QUEUE[] = {
    // 棋盘事件
    {
        .x = 0,
        .y = OFFY,
        .w = 240,
        .h = 267,
        .filter = GAME_PLAYING,
        .handler = ev_click_chess,
    },
    // 开始页面
    {
        .x = 10,
        .y = 115,
        .w = 240,
        .h = 30,
        .filter = NOT_BEGIN,
        .handler = ev_click_welcome,
    },
    {
        .x = 10,
        .y = 115 + 50,
        .w = 240,
        .h = 30,
        .filter = NOT_BEGIN,
        .handler = ev_click_welcome,
    },
};
u8 EV_LEN = LEN(EV_QUEUE);

// 游戏棋盘
u8 chess_board[16 * 16];
// 当前走子选手
u8 current_player = 0;
// 棋子选中
u8 sq_selected = 0;
// 上一步走子记录
u16 mv_last = 0;
// 走子历史记录(stack)
struct move_t chess_moves[MAX_MOVE_LEN];
u16 move_index = 0;

struct player players[] = {
    {
        .lefttime = 15 * 60 * 1000,  // 15min,
    },
    {
        .lefttime = 15 * 60 * 1000,  // 15min
    },
};

#define player players[current_player]

void draw_res(u16 x, u16 y, const tImage res) {
  LCD_Color_Fill(x, y, x + res.width - 1, y + res.height - 1, (u16 *)res.data);
}

void draw_chess_res(u16 x, u16 y, const tImage res) {
  u16 color[24 * 24];
  for (u16 i = 0; i < 24; i++) {
    for (u16 j = 0; j < 24; j++) {
      u16 c = res.data[i * 24 + j];
      if (c == WHITE) {
        c = LCD_RES_BOARD.data[(y - OFFY + i) * 240 + (x + j)];
      }
      color[i * 24 + j] = c;
    }
  }
  LCD_Color_Fill(x, y, x + 23, y + 23, color);
}

void clear_chess_res(u16 x, u16 y) {
  u16 color[24 * 24];
  for (u16 i = 0; i < 24; i++) {
    for (u16 j = 0; j < 24; j++) {
      color[i * 24 + j] = LCD_RES_BOARD.data[(y - OFFY + i) * 240 + (x + j)];
    }
  }
  LCD_Color_Fill(x, y, x + 23, y + 23, color);
}

/**
 * 画一个棋子
 */
#define DRAW_SELECTED 1
#define DRAW_UNSELECT 0
void draw_piece(u8 sq, u8 selected) {
  u16 xx = BOARD_EDGE + (FILE_X(sq) - FILE_LEFT) * SQUARE_SIZE;
  u16 yy = BOARD_EDGE + (RANK_Y(sq) - RANK_TOP) * SQUARE_SIZE;
  u8 pc = chess_board[sq];
  yy += OFFY;
  if (pc) {
    draw_chess_res(xx, yy, PIECE_RES(pc));
  } else {
    clear_chess_res(xx, yy);
  }

  if (selected) {
    LCD_Color_Fillx(xx, yy, xx + 23, yy + 23, (u16 *)LCD_RES_SELECT.data,
                    WHITE);
  }
}

void welcome_page(void) {
  CC_STATE = NOT_BEGIN;
  // background
  draw_res(0, OFFY, LCD_RES_BOARD);

  POINT_COLOR = RED;
  LCD_PRINT_SIZE = 24;
  LCD_Draw_printf(35, 60, "Chinese Chess");

  LCD_PRINT_SIZE = 16;
  POINT_COLOR = BLACK;
  LCD_DrawRectangle(65, 115, 180, 140);
  LCD_DrawRectangle(65, 115 + 50, 180, 140 + 50);
  LCD_Draw_printf(70, 120, "Play Computer");
  LCD_Draw_printf(70, 170, "Play Personal");
  draw_chess_res(40, 90 + OFFY, LCD_RES_BK);
  draw_chess_res(40, 90 + 50 + OFFY, LCD_RES_RK);
}

void init_draw(void) {
  draw_res(0, OFFY, LCD_RES_BOARD);

  for (u8 x = FILE_LEFT; x <= FILE_RIGHT; x++) {
    for (u8 y = RANK_TOP; y <= RANK_BOTTOM; y++) {
      u16 xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
      u16 yy = BOARD_EDGE + (y - RANK_TOP) * SQUARE_SIZE;
      u8 sq = COORD_XY(x, y);
      u8 pc = chess_board[sq];
      if (pc) {
        draw_chess_res(xx, yy + OFFY, PIECE_RES(pc));
      }
    }
  }

  draw_res(15, 0, LCD_RES_BK);
  draw_res(15, 294, LCD_RES_RK);
}

void chess_init(void) {
  current_player = PLAYER_RED;
  CC_STATE = GAME_PLAYING;
  memcpy(chess_board, CC_BOARD_STARTUP, sizeof(chess_board));

  if (CC_MODE == PLAYER_VS_COMPUTER) {
    // 5min
    players[PLAYER_BLACK].lefttime = 5 * 60 * 1000;
    init_draw();
    LCD_PRINT_MODE = 1;
    LCD_PRINT_SIZE = 16;
    LCD_Draw_printf(18, 150, "Waiting UCCI OK ...");
    UCCI_FirstSync();
  }

  init_draw();
}

void ev_click_welcome(event_t *e) {
  // u16 x = e->param1;
  u16 y = e->param2;
  if (y <= 115 + 50) {
    // Computer

    CC_MODE = PLAYER_VS_COMPUTER;
    chess_init();

  } else {
    // Personal
    CC_MODE = PLAYER_VS_PLAYER;
    chess_init();
  }
}

// 改变玩家
void change_side(void) { current_player = 1 - current_player; }

// 棋盘添加棋子
inline void add_piece(u8 sq, u8 pc) { chess_board[sq] = pc; }

// 棋盘删除棋子
inline void rm_piece(u8 sq) { chess_board[sq] = 0; }

// 棋盘移动棋子
u8 move_piece(u16 mv) {
  u8 sq_src = SRC(mv), sq_dst = DST(mv);
  u8 pc_src = chess_board[sq_src];
  u8 pc_dst = chess_board[sq_dst];
  rm_piece(sq_src);
  rm_piece(sq_dst);
  add_piece(sq_dst, pc_src);
  return pc_dst;
}

BOOL is_checked(void) {
  s16 sq_dst, pc_dst, sq_delta;

  u8 pc_self_side = SIDE_TAG(current_player);
  u8 pc_opp_side = OPP_SIDE_TAG(current_player);

  for (u16 sq_src = 0; sq_src < 256; sq_src++) {
    // 0. 找到自己的将/帅
    if (chess_board[sq_src] != PIECE_KING + pc_self_side) {
      continue;
    }
    // printf("FOUND king at(%d,%d)\n", FILE_X(sq_src), RANK_Y(sq_src));

    // 1. 是否被对方卒将
    // printf("1. check PAWN\n");
    if (chess_board[SQUARE_FORWARD(sq_src, current_player)] ==
            pc_opp_side + PIECE_PAWN ||
        chess_board[sq_src - 1] == pc_opp_side + PIECE_PAWN ||
        chess_board[sq_src + 1] == pc_opp_side + PIECE_PAWN) {
      return TRUE;
    }

    // 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
    // printf("2. check KNIGHT\n");
    for (u8 i = 0; i < 4; i++) {
      if (chess_board[sq_src + CC_ADVISOR_DELTA[i]] != 0) {
        continue;  // 马腿
      }

      for (u8 j = 0; j < 2; j++) {
        if (chess_board[sq_src + CC_KNIGHT_CHECK_DELTA[i][j]] ==
            pc_opp_side + PIECE_KNIGHT) {
          return TRUE;
        }
      }
    }

    // 3. 判断是否被对方的车或炮将军(包括将帅对脸)
    // printf("3. check ROOK/CANNON/KING\n");
    for (u8 i = 0; i < 4; i++) {
      sq_delta = CC_KING_DELTA[i];
      sq_dst = sq_src + sq_delta;
      // printf(" delta: %d\n", sq_delta);
      while (IN_BOARD(sq_dst)) {
        pc_dst = chess_board[sq_dst];
        // printf("  d: %d, dst: %d->(%d,%d), chess: %s\n", sq_delta, sq_dst,
        //  FILE_X(sq_dst), RANK_Y(sq_dst), PIECE_NAME(pc_dst));
        if (pc_dst != 0) {
          if (pc_dst == pc_opp_side + PIECE_ROOK ||
              pc_dst == pc_opp_side + PIECE_KING) {
            return TRUE;
          }
          break;
        }
        sq_dst += sq_delta;
      }
      sq_dst += sq_delta;
      // printf(" crossover\n");
      while (IN_BOARD(sq_dst)) {
        pc_dst = chess_board[sq_dst];
        // printf("  dst: %d->(%d,%d), chess: %s\n", sq_dst, FILE_X(sq_dst),
        //  RANK_Y(sq_dst), PIECE_NAME(pc_dst));
        if (pc_dst != 0) {
          if (pc_dst == pc_opp_side + PIECE_CANNON) {
            return TRUE;
          }
          break;
        }
        sq_dst += sq_delta;
      }
    }
    return FALSE;
  }
  return FALSE;
}

void undo_move_piece(u16 mv, u16 pc) {
  u8 sq_src = SRC(mv), sq_dst = DST(mv);
  u8 pc_dst = chess_board[sq_dst];
  rm_piece(sq_dst);
  add_piece(sq_dst, pc);
  add_piece(sq_src, pc_dst);
}

BOOL make_move(u16 mv) {
  u8 pc = move_piece(mv);
  if (is_checked()) {
    undo_move_piece(mv, pc);
    return FALSE;
  }
  change_side();
  return TRUE;
}

/**
 * 判断走法是否合法(ok)
 */
BOOL is_legal_move(u16 mv) {
  u16 sq_src, sq_dst, sq_pin;
  u16 pc_self_side, pc_src, pc_dst;
  s16 sq_delta;

  /**
   * 1. 判断起始点是自己的棋子
   */
  sq_src = SRC(mv);
  pc_src = chess_board[sq_src];
  pc_self_side = SIDE_TAG(current_player);
  if (!(pc_self_side & pc_src)) {
    return FALSE;
  }

  /**
   * 2. 目标是否是自己的棋子
   */
  sq_dst = DST(mv);
  pc_dst = chess_board[sq_dst];
  if (pc_self_side & pc_dst) {
    return FALSE;
  }

  /**
   * 3. 根据棋子类型判断走法是否合理
   */
  switch (pc_src - pc_self_side) {
    case PIECE_KING:  // 将
      return IN_FORT(sq_dst) && KING_SPAN(sq_src, sq_dst);

    case PIECE_ADVISOR:  // 士
      return IN_FORT(sq_dst) && ADVISOR_SPAN(sq_src, sq_dst);

    case PIECE_BISHOP:  // 象
      return SAME_HALF(sq_src, sq_dst) && BISHOP_SPAN(sq_src, sq_dst) &&
             !HAS_PIECE(BISHOP_PIN(sq_src, sq_dst));

    case PIECE_KNIGHT:  // 马
      sq_pin = KNIGHT_PIN(sq_src, sq_dst);
      return sq_pin != sq_src && !HAS_PIECE(sq_pin);

    case PIECE_ROOK:
    case PIECE_CANNON:  // 车、炮
      if (SAME_RANK(sq_src, sq_dst)) {
        sq_delta = (sq_dst < sq_src ? -1 : 1);
      } else if (SAME_FILE(sq_src, sq_dst)) {
        sq_delta = (sq_dst < sq_src ? -16 : 16);
      } else {
        return FALSE;
      }
      sq_pin = sq_src + sq_delta;
      while (sq_pin != sq_dst && !HAS_PIECE(sq_pin)) {
        sq_pin += sq_delta;
      }
      if (sq_pin == sq_dst) {
        return pc_dst == 0 || pc_src - pc_self_side == PIECE_ROOK;
      } else if (pc_dst != 0 && pc_src - pc_self_side == PIECE_CANNON) {
        sq_pin += sq_delta;
        while (sq_pin != sq_dst && !HAS_PIECE(sq_pin)) {
          sq_pin += sq_delta;
        }
        return sq_pin == sq_dst;
      } else {
        return FALSE;
      }

    case PIECE_PAWN:
      if (AWAY_HALF(sq_src, current_player) &&
          (sq_dst == sq_src - 1 || sq_dst == sq_src + 1)) {
        return TRUE;
      }
      return sq_dst == SQUARE_FORWARD(sq_src, current_player);
  }

  return FALSE;
}

/**
 * 生成当前所有走法
 */
u16 generate_moves(u16 *mvs) {
  u8 pc_self_side = SIDE_TAG(current_player);
  u8 pc_opp_side = OPP_SIDE_TAG(current_player);
  u16 gen_mvs = 0;

  u8 pc_dst, sq_dst;
  for (u16 sq_src = 0; sq_src < 256; sq_src++) {
    // 1. 找到本方的棋子
    u8 pc_src = chess_board[sq_src];
    if ((pc_src & pc_self_side) == 0) {
      continue;
    }

    // 2. 根据棋子确定走法
    u8 i, j;
    switch (pc_src - pc_self_side) {
      case PIECE_KING:
        for (i = 0; i < 4; i++) {
          sq_dst = sq_src + CC_KING_DELTA[i];
          if (!IN_FORT(sq_dst)) {
            continue;
          }

          pc_dst = chess_board[sq_dst];
          if (!(pc_dst & pc_self_side)) {
            mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
          }
        }
        break;

      case PIECE_ADVISOR:
        for (i = 0; i < 4; i++) {
          sq_dst = sq_src + CC_ADVISOR_DELTA[i];
          if (!IN_FORT(sq_dst)) {
            continue;
          }

          pc_dst = chess_board[sq_dst];
          if (!(pc_dst & pc_self_side)) {
            mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
          }
        }
        break;

      case PIECE_BISHOP:
        for (i = 0; i < 4; i++) {
          sq_dst = sq_src + CC_ADVISOR_DELTA[i];
          if (!(IN_BOARD(sq_dst) && HOME_HALF(sq_dst, current_player) &&
                chess_board[sq_dst] == 0)) {
            continue;
          }

          sq_dst += CC_ADVISOR_DELTA[i];
          pc_dst = chess_board[sq_dst];
          if (!(pc_dst & pc_self_side)) {
            mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
          }
        }
        break;

      case PIECE_KNIGHT:
        for (i = 0; i < 4; i++) {
          sq_dst = sq_src + CC_KING_DELTA[i];
          if (chess_board[sq_dst]) {
            continue;
          }

          for (j = 0; j < 2; j++) {
            sq_dst = sq_src + CC_KNIGHT_DELTA[i][j];
            if (!IN_BOARD(sq_dst)) {
              continue;
            }

            pc_dst = chess_board[sq_dst];
            if (!(pc_dst & pc_self_side)) {
              mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
            }
          }
        }
        break;

      case PIECE_ROOK:
        for (i = 0; i < 4; i++) {
          s8 delta = CC_KING_DELTA[i];
          sq_dst = sq_src + delta;
          while (IN_BOARD(sq_dst)) {
            pc_dst = chess_board[sq_dst];
            if (pc_dst == 0) {
              mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
            } else {
              if (pc_dst & pc_opp_side) {
                mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
              }
              break;
            }
            sq_dst += delta;
          }
        }
        break;

      case PIECE_CANNON:
        for (i = 0; i < 4; i++) {
          s8 delta = CC_KING_DELTA[i];
          sq_dst = sq_src + delta;
          while (IN_BOARD(sq_dst)) {
            pc_dst = chess_board[sq_dst];
            if (pc_dst == 0) {
              mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
            } else {
              break;
            }
            sq_dst += delta;
          }

          sq_dst += delta;
          while (IN_BOARD(sq_dst)) {
            pc_dst = chess_board[sq_dst];
            if (pc_dst) {
              if (pc_dst & pc_opp_side) {
                mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
              }
              break;
            }
            sq_dst += delta;
          }
        }
        break;

      case PIECE_PAWN:
        sq_dst = SQUARE_FORWARD(sq_src, current_player);
        if (IN_BOARD(sq_dst)) {
          pc_dst = chess_board[sq_dst];
          if (!(pc_dst & pc_self_side)) {
            mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
          }
        }

        if (AWAY_HALF(sq_src, current_player)) {
          for (s8 delta = -1; delta < 1; delta += 2) {
            sq_dst = sq_src + delta;
            if (IN_BOARD(sq_dst)) {
              pc_dst = chess_board[sq_dst];
              if (!(pc_dst & pc_self_side)) {
                mvs[gen_mvs++] = MOVE(sq_src, sq_dst);
              }
            }
          }
        }
        break;
    }
  }

  // log_debug("%s: gen_mvs: %d\n", PLAYER_NAME(current_player), gen_mvs);
  return gen_mvs;
}

/**
 * 是否绝杀无解
 */
BOOL is_mate(void) {
  u16 mvs[MAX_GEN_MOVES];
  u16 gen_movs = generate_moves(mvs);
  for (u8 i = 0; i < gen_movs; i++) {
    u8 pc_store = move_piece(mvs[i]);
    BOOL checked = is_checked();
    undo_move_piece(mvs[i], pc_store);
    if (!checked) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * 电脑走子
 */
void EngineMove(u16 mv) {
  if (is_legal_move(mv)) {
    u8 pc = chess_board[DST(mv)];
    if (make_move(mv)) {
      if (mv_last != 0) {
        // clear 上一步的提示
        draw_piece(SRC(mv_last), DRAW_UNSELECT);
        draw_piece(DST(mv_last), DRAW_UNSELECT);
      }

      mv_last = mv;
      sq_selected = 0;
      draw_piece(DST(mv), DRAW_SELECTED);
      draw_piece(SRC(mv), DRAW_SELECTED);

      // 保存走子信息
      chess_moves[move_index++] = (struct move_t){.mv = mv, .pc = pc};

      if (is_mate()) {
        // 是否分出胜负
        CC_STATE = GAME_WIN;
      } else {
        if (is_checked()) {
          BEEP_INTERVAL = 500;
        }
      }
    }
  } else {
    BEEP_INTERVAL = 500;
    // 非法走棋子，忽略不处理
  }
}

/**
 * 点击棋子处理逻辑
 */
void click_chess(u8 sq) {
  u8 pc = chess_board[sq];
  log_debug("click: %s %s (%d,%d)\n", PLAYER_NAME(current_player),
            PIECE_NAME(pc), FILE_X(sq), RANK_Y(sq));

  if (CC_MODE == PLAYER_VS_COMPUTER && current_player == PLAYER_BLACK) {
    // 等待电脑走棋
    return;
  }

  if ((pc & SIDE_TAG(current_player)) != 0) {
    // 点击自己的子
    if (mv_last != 0) {
      // clear 上一步的提示
      draw_piece(SRC(mv_last), DRAW_UNSELECT);
      draw_piece(DST(mv_last), DRAW_UNSELECT);
    }
    if (sq_selected != 0) {
      // 如果之前有选择，现在清除选择
      draw_piece(sq_selected, DRAW_UNSELECT);
    }
    sq_selected = sq;
    draw_piece(sq, DRAW_SELECTED);
    // play sound
    BEEP_INTERVAL = 20;

  } else if (sq_selected != 0) {
    // 如果点击的不是自己的子
    // 但有子选中了(一定是自己的子)，那么走这个子
    u16 mv = MOVE(sq_selected, sq);
    if (is_legal_move(mv)) {
      if (make_move(mv)) {
        mv_last = mv;
        draw_piece(sq_selected, DRAW_SELECTED);
        draw_piece(sq, DRAW_SELECTED);
        sq_selected = 0;

        // 保存走子信息
        chess_moves[move_index++] = (struct move_t){.mv = mv, .pc = pc};

        if (is_mate()) {
          // 是否分出胜负
          CC_STATE = GAME_WIN;
        } else {
          if (is_checked()) {
            BEEP_INTERVAL = 500;
          }

          // computer
          if (CC_MODE == PLAYER_VS_COMPUTER) {
            // 1. 发送当前局面和走子信息
            UCCI_Command(UCCI_MOVE, NULL);
            // 2. 等待电脑走子信息
            // 由 EngineMove 触发
          }
        }
      } else {
        // 被将军 提示
        BEEP_INTERVAL = 500;
      }
    } else {
      // 非法走棋子，忽略不处理
    }
  }
}

void Game_Init(void) { welcome_page(); }

void Game_Update(u16 elasped) {
  // printf("game_update");
  if (CC_STATE == GAME_PLAYING) {
    if (CC_MODE == PLAYER_VS_COMPUTER) {
      UCCI_SyncUpdate();
    }
    player.lefttime -= elasped;
    u16 sec = player.lefttime / 1000;
    u16 min = sec / 60;
    LCD_PRINT_MODE = 0;
    LCD_PRINT_SIZE = 16;
    if (current_player == PLAYER_BLACK) {
      LCD_Draw_printf(10 + 35, 3, "%02d:%02d", min, sec % 60);
    } else {
      LCD_Draw_printf(10 + 35, 297, "%02d:%02d", min, sec % 60);
    }
    if (player.lefttime < 0) {
      CC_STATE = GAME_WIN;
    }
  } else if (CC_STATE == GAME_WIN) {
    POINT_COLOR = RED;
    LCD_PRINT_MODE = 1;
    LCD_PRINT_SIZE = 24;
    LCD_Draw_printf(18, 150, "%s WINNER!", PLAYER_NAME(1 - current_player));
  } else if (CC_STATE == GAME_DRAW) {
    POINT_COLOR = RED;
    LCD_PRINT_MODE = 1;
    LCD_PRINT_SIZE = 24;
    LCD_Draw_printf(18, 150, "DRAW~");
  }
}

void Game_Event(event_t *e) {
  if (e->type == EVENT_Touchdown) {
    u16 x = e->param1, y = e->param2;
    for (u8 i = 0; i < EV_LEN; i++) {
      struct ev_area_t *ev = EV_QUEUE + i;
      if (ev->filter == CC_STATE) {
        if (x >= ev->x && x <= ev->x + ev->w && y >= ev->y &&
            y <= ev->y + ev->h) {
          ev->handler(e);
          break;
        }
      }
    }
  } else if (e->type == EVENT_Keypress) {
    ev_undo_chess();
  }
}

void ev_click_chess(event_t *e) {
  /**
   * 0 <= x <= 240
   * 0 <= y <= 267
   */
  u16 x = e->param1, y = e->param2;
  y -= OFFY;
  u8 px = FILE_LEFT + (x - BOARD_EDGE) / SQUARE_SIZE;
  u8 py = RANK_TOP + (y - BOARD_EDGE) / SQUARE_SIZE;
  if (px >= FILE_LEFT && px <= FILE_RIGHT && py >= RANK_TOP &&
      py <= RANK_BOTTOM) {
    click_chess(COORD_XY(px, py));
  }
}

void ev_undo_chess(void) {
  if (CC_STATE != GAME_PLAYING) return;
  // 悔棋
  if (move_index == 0) {
    // 步数不够... 无法悔棋
    return;
  }
  if (CC_MODE == PLAYER_VS_COMPUTER && current_player == PLAYER_BLACK) {
    // 电脑思考中... 无法悔棋
    return;
  }
  if (sq_selected) {
    draw_piece(sq_selected, DRAW_UNSELECT);
  }
  if (mv_last != 0) {
    // clear 上一步的提示
    draw_piece(SRC(mv_last), DRAW_UNSELECT);
    draw_piece(DST(mv_last), DRAW_UNSELECT);
  }
  struct move_t move = chess_moves[--move_index];
  undo_move_piece(move.mv, move.pc);
  mv_last = move.mv;
  draw_piece(SRC(move.mv), DRAW_SELECTED);
  draw_piece(DST(move.mv), DRAW_SELECTED);
  change_side();
  if (CC_MODE == PLAYER_VS_COMPUTER) {
    // clear 上一步的提示
    draw_piece(SRC(mv_last), DRAW_UNSELECT);
    draw_piece(DST(mv_last), DRAW_UNSELECT);

    move = chess_moves[--move_index];
    undo_move_piece(move.mv, move.pc);
    mv_last = move.mv;
    draw_piece(SRC(move.mv), DRAW_SELECTED);
    draw_piece(DST(move.mv), DRAW_SELECTED);
    change_side();
  }
  sq_selected = 0;
  BEEP_INTERVAL = 100;
}
