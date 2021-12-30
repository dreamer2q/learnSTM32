#ifndef __CHESS_H
#define __CHESS_H

#include "lcd.h"

void GAME_printf(const char* fmt, ...);

/**
 * 全局变量
 */
#define MAX_MOVE_LEN 512
struct move_t {
  u16 mv;
  u8 pc;
};
extern struct move_t chess_moves[MAX_MOVE_LEN];
extern u16 move_index;

/**
 * 全局常量
 */
// 棋盘初始设置
extern const s8 CC_BOARD_STARTUP[256];
// 判断棋子是否在棋盘中的数组
extern const s8 CC_IN_BOARD[256];
// 判断棋子是否在九宫的数组
extern const s8 CC_IN_FORT[256];
// 判断步长是否符合特定走法的数组，1=帅(将)，2=仕(士)，3=相(象)
extern const s8 CC_LEGAL_SPAN[512];
// 根据步长判断马是否蹩腿的数组
extern const s8 CC_KNIGHT_PIN[512];
// 帅(将)的步长
extern const s8 CC_KING_DELTA[4];
// 仕(士)的步长
extern const s8 CC_ADVISOR_DELTA[4];
// 马的步长，以帅(将)的步长作为马腿
extern const s8 CC_KNIGHT_DELTA[4][2];
// 马被将军的步长，以仕(士)的步长作为马腿
extern const s8 CC_KNIGHT_CHECK_DELTA[4][2];

// 当前棋盘
// extern u8 chess_board[256];

// 棋盘范围
#define RANK_TOP 3
#define RANK_BOTTOM 12
#define FILE_LEFT 3
#define FILE_RIGHT 11
#define MASK_COLOR WHITE
#define SQUARE_SIZE 26
#define BOARD_EDGE 4
// #define BOARD_WIDTH (BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE)
// #define BOARD_HEIGHT (BOARD_EDGE + SQUARE_SIZE * 10 + BOARD_EDGE)

// 棋子编号
#define PIECE_KING 0
#define PIECE_ADVISOR 1
#define PIECE_BISHOP 2
#define PIECE_KNIGHT 3
#define PIECE_ROOK 4
#define PIECE_CANNON 5
#define PIECE_PAWN 6

// Player 编号
#define PLAYER_RED 0
#define PLAYER_BLACK 1

/**
 * 获取棋子名称
 */
const char* PIECE_NAME(u8 pc);

/**
 * 获取棋子资源
 */
const tImage PIECE_RES(u8 pc);

/**
 * Player
 */
struct player {
  s32 lefttime;
};
extern struct player players[];
extern u8 current_player;

/**
 * 获取 Player 昵称
 */
inline const char* PLAYER_NAME(u8 player) {
  if (player == PLAYER_BLACK) {
    return "Black";
  }
  return "Red";
}

// 最大生成走法数
#define MAX_GEN_MOVES 128

// 获得格子的横坐标
// #define RANK_Y(sq) (((sq) >> 4) & 0xF)
#define RANK_Y(sq) ((sq) >> 4)

// 获得格子的纵坐标
#define FILE_X(sq) ((sq)&0xF)

// 根据纵坐标和横坐标获得格子
#define COORD_XY(x, y) ((x) + ((y) << 4))

// 翻转格子
#define SQUARE_FLIP(sq) (254 - sq)

// 纵坐标水平镜像
#define FILE_FLIP(x) (14 - x)

// 横坐标垂直镜像
#define RANK_FLIP(y) (15 - y)

// 获得红黑标记(红子是8，黑子是16)
#define SIDE_TAG(sd) (8 + ((sd) << 3))

// 获得对方红黑标记
#define OPP_SIDE_TAG(sd) (16 - ((sd) << 3))

// 获得走法的起点
#define SRC(mv) ((mv)&0xFF)

// 获得走法的终点
#define DST(mv) ((mv) >> 8)

// 根据起点和终点获得走法
#define MOVE(sqSrc, sqDst) ((sqSrc) + ((sqDst) << 8))

// 判断棋子是否在棋盘中
inline BOOL IN_BOARD(u8 sq) { return CC_IN_BOARD[sq] != 0; }

// 是否向对方前进一步
inline u8 SQUARE_FORWARD(u8 sq, u8 sd) { return sq - 16 + (sd << 5); }

// 判断棋子是否在九宫中
inline BOOL IN_FORT(u8 sq) { return CC_IN_FORT[sq] != 0; }

// 马腿的位置
inline u8 KNIGHT_PIN(u8 sqSrc, u8 sqDst) {
  return sqSrc + CC_KNIGHT_PIN[sqDst - sqSrc + 256];
}

// 相(象)眼的位置
inline u8 BISHOP_PIN(u16 sqSrc, u16 sqDst) { return (sqSrc + sqDst) >> 1; }

// 是否在同一行
inline BOOL SAME_RANK(u8 sqSrc, u8 sqDst) {
  return ((sqSrc ^ sqDst) & 0xf0) == 0;
}

// 是否在同一列
inline BOOL SAME_FILE(int sqSrc, int sqDst) {
  return ((sqSrc ^ sqDst) & 0x0f) == 0;
}

// 是否未过河
inline BOOL HOME_HALF(u8 sq, u8 sd) { return (sq & 0x80) != (sd << 7); }

// 是否已过河
inline BOOL AWAY_HALF(u8 sq, u8 sd) { return (sq & 0x80) == (sd << 7); }

// 是否在河的同一边
inline BOOL SAME_HALF(u8 sqSrc, u8 sqDst) {
  return ((sqSrc ^ sqDst) & 0x80) == 0;
}

// 走法是否符合帅(将)的步长
inline BOOL KING_SPAN(u16 sqSrc, u16 sqDst) {
  return CC_LEGAL_SPAN[sqDst - sqSrc + 256] == 1;
}

// 走法是否符合仕(士)的步长
inline BOOL ADVISOR_SPAN(u16 sqSrc, u16 sqDst) {
  return CC_LEGAL_SPAN[sqDst - sqSrc + 256] == 2;
}

// 走法是否符合相(象)的步长
inline BOOL BISHOP_SPAN(u16 sqSrc, u16 sqDst) {
  return CC_LEGAL_SPAN[sqDst - sqSrc + 256] == 3;
}

/**
 * UCCI
 */
void EngineMove(u16 mv);

struct ucci_stat {
  u8 ucciok;
  struct ucci_id {
    char name[32];
    char version[8];
    char user[32];
  } id;
};

enum ucci_command {
  UCCI_CUSTOM,
  UCCI_INIT,
  UCCI_MOVE,
};

extern struct ucci_stat ucci_com;

void UCCI_FirstSync(void);
void UCCI_SyncUpdate(void);
void UCCI_Command(enum ucci_command cmd, const char* args);
void UCCI_Parse(const char* info);

#endif
