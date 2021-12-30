#include "header.h"
#include "chess.h"
#include <stdio.h>
#include <string.h>

#define COM_MAXLEN 2048
// 5 分钟
#define UCCI_MAXTIME 300000
#define compare(s1, op, s2) (strcmp(s1, s2) op 0)

/**
 * 全局变量
 */
char UCCI_BUF[COM_MAXLEN];
u16 BUFLEN = 0;
struct ucci_stat ucci_com = {
    .ucciok = 0,
};

const char* move2ucci(int mv) {
  static char info[5] = {0};
  // eg: b0c2 (b0 -> src, c2 -> dst)
  // x: a b c d e f g h i (from left to right)
  // y: 0 1 2 3 4 5 6 7 8 9 (from bottom to top)
  info[0] = (FILE_X(SRC(mv) - FILE_LEFT)) + 'a';
  info[1] = '9' - (RANK_Y(SRC(mv)) - RANK_TOP);
  info[2] = (FILE_X(DST(mv) - FILE_LEFT)) + 'a';
  info[3] = '9' - (RANK_Y(DST(mv)) - RANK_TOP);
  return info;
}

int ucci2move(const char* info) {
  // eg: b0c2 (b0 -> src, c2 -> dst)
  // x: a b c d e f g h i (from left to right)
  // y: 0 1 2 3 4 5 6 7 8 9 (from bottom to top)
  u8 src_x = FILE_LEFT + (info[0] - 'a');
  u8 src_y = RANK_BOTTOM - (info[1] - '0');
  u8 dst_x = FILE_LEFT + (info[2] - 'a');
  u8 dst_y = RANK_BOTTOM - (info[3] - '0');
  u16 mv = MOVE(COORD_XY(src_x, src_y), COORD_XY(dst_x, dst_y));
  return mv;
}

void UCCI_Command(enum ucci_command cmd, const char* args) {
  switch (cmd) {
    case UCCI_INIT:
      printf("ucci\n");
      printf("isready\n");
      break;

    case UCCI_MOVE:
      printf("position startpos - - 0 1 moves");
      for (int i = 0; i < move_index; i++) {
        printf(" %s", move2ucci(chess_moves[i].mv));
      }
      printf("\n");
      printf("go time %ld increment 0\n", players[current_player].lefttime);
      break;

    default:
      if (args != NULL) {
        printf("%s\n", args);
      }
      break;
  }
}

void UCCI_FirstSync(void) {
  do {
    UCCI_Command(UCCI_INIT, NULL);
    UCCI_SyncUpdate();
    delay_ms(1000);
  } while (!ucci_com.ucciok);
}

void UCCI_SyncUpdate(void) {
  while (USART_haschar()) {
    int len = USART_readline(UCCI_BUF + BUFLEN, sizeof(UCCI_BUF) - BUFLEN);
    if (len > 0) {
      BUFLEN += len;
      if (UCCI_BUF[BUFLEN - 1] == '\n') {
        UCCI_BUF[BUFLEN - 1] = '\0';
        BUFLEN = 0;
        // GAME_printf("%c", UCCI_BUF[0]);
        UCCI_Parse(UCCI_BUF);
      }
    } else {
      printf("readline error\n");
    }
  }
}

u8 hasprefix(const char* str, const char* prefix) {
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

#define istype(type) hasprefix(info, type)

const char type_readyok[] = "readyok";
const char type_ucciok[] = "ucciok";
const char type_bye[] = "bye";

const char type_info[] = "info";
const char type_info_time[] = "time";
const char type_id[] = "id";
const char type_id_name[] = "name";
const char type_id_version[] = "version";
const char type_id_user[] = "user";
const char type_option[] = "option";
const char type_bestmove[] = "bestmove";
const char type_nobestmove[] = "nobestmove";
const char type_pophash[] = "pophash";

void UCCI_Parse(const char* info) {
  // GAME_printf(info);
  if (istype(type_ucciok)) {
    ucci_com.ucciok = 1;
  } else if (istype(type_id)) {
    info += sizeof type_id;
    if (hasprefix(info, type_id_name)) {
      strcpy(ucci_com.id.name, info + sizeof type_id_name);
    } else if (hasprefix(info, type_id_version)) {
      strcpy(ucci_com.id.name, info + sizeof type_id_version);
    } else if (hasprefix(info, type_id_user)) {
      strcpy(ucci_com.id.name, info + sizeof type_id_user);
    }

  } else if (istype(type_info)) {
    info += sizeof type_info;
    // 获取思考信息
    // if (hasprefix(info, type_info_time)) {
    // sscanf(info + sizeof type_info_time, "%d", &last_think_time);
    // }
  } else if (istype(type_option)) {
    // skip

  } else if (istype(type_bestmove)) {
    info += sizeof type_bestmove;
    EngineMove(ucci2move(info));
  } else if (istype(type_nobestmove)) {
    // skip

  } else if (istype(type_readyok)) {
    ucci_com.ucciok = 1;

  } else if (istype(type_pophash)) {
    // skip

  } else if (istype(type_bye)) {
    ucci_com.ucciok = 0;

  } else {
    // not recoganized
  }
}
