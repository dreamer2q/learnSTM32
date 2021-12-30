
#ifndef __LCD_H
#define __LCD_H

#include <stdlib.h>
#include "sys.h"
#include "header.h"

/**
 * LCD 资源
 */
typedef struct lcd_image_t {
  const uint16_t *data;
  const uint16_t width;
  const uint16_t height;
  const uint8_t dataSize;
} tImage;

extern const tImage LCD_RES_BOARD;
extern const tImage LCD_RES_BA;
extern const tImage LCD_RES_BB;
extern const tImage LCD_RES_BC;
extern const tImage LCD_RES_BK;
extern const tImage LCD_RES_BN;
extern const tImage LCD_RES_BP;
extern const tImage LCD_RES_BR;
extern const tImage LCD_RES_RA;
extern const tImage LCD_RES_RB;
extern const tImage LCD_RES_RC;
extern const tImage LCD_RES_RK;
extern const tImage LCD_RES_RN;
extern const tImage LCD_RES_RP;
extern const tImage LCD_RES_RR;
extern const tImage LCD_RES_SELECT;

/**
 * TOUCH 触摸屏
 */
extern u16 TOUCH_LAST_X, TOUCH_LAST_Y;
// extern void (*TOUCH_Handler)(event_t *e);
void TOUCH_Configure(void);

/**
 * LCD 屏幕
 */

// LCD重要参数集
typedef struct {
  u16 width;    // LCD 宽度
  u16 height;   // LCD 高度
  u16 id;       // LCD ID
  u8 dir;       //横屏还是竖屏控制：0，竖屏；1，横屏。
  u16 wramcmd;  //开始写gram指令
  u16 setxcmd;  //设置x坐标指令
  u16 setycmd;  //设置y坐标指令
} _lcd_dev;

// LCD参数
extern _lcd_dev lcddev;  //管理LCD重要参数
// LCD的画笔颜色和背景色
extern u16 POINT_COLOR;  //默认红色
extern u16 BACK_COLOR;   //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------
#define LCD_LED PCout(0)  // LCD背光    	PC0

#define LCD_CS_SET GPIOD->BSRR = 1 << 7   //片选端口  		PD7
#define LCD_RD_SET GPIOD->BSRR = 1 << 11  //数据/命令 		PD12
#define LCD_WR_SET GPIOD->BSRR = 1 << 5   //写数据			PD5
#define LCD_RS_SET GPIOD->BSRR = 1 << 4   //读数据			PD4

#define LCD_CS_CLR GPIOD->BRR = 1 << 7   //片选端口  		PD7
#define LCD_RD_CLR GPIOD->BRR = 1 << 11  //数据/命令		PD12
#define LCD_WR_CLR GPIOD->BRR = 1 << 5   //写数据			PD5
#define LCD_RS_CLR GPIOD->BRR = 1 << 4   //读数据			PD4

//////////////////////////////////////////////////////////////////////
//扫描方向定义
#define L2R_U2D 0  //从左到右,从上到下
#define L2R_D2U 1  //从左到右,从下到上
#define R2L_U2D 2  //从右到左,从上到下
#define R2L_D2U 3  //从右到左,从下到上

#define U2D_L2R 4  //从上到下,从左到右
#define U2D_R2L 5  //从上到下,从右到左
#define D2U_L2R 6  //从下到上,从左到右
#define D2U_R2L 7  //从下到上,从右到左

#define DFT_SCAN_DIR L2R_U2D  //默认的扫描方向

//扫描方向定义
#define L2R_U2D 0  //从左到右,从上到下
#define L2R_D2U 1  //从左到右,从下到上
#define R2L_U2D 2  //从右到左,从上到下
#define R2L_D2U 3  //从右到左,从下到上

#define U2D_L2R 4  //从上到下,从左到右
#define U2D_R2L 5  //从上到下,从右到左
#define D2U_L2R 6  //从下到上,从左到右
#define D2U_R2L 7  //从下到上,从右到左

#define DFT_SCAN_DIR L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40  //棕色
#define BRRED 0XFC07  //棕红色
#define GRAY 0X8430   //灰色
// GUI颜色

#define DARKBLUE 0X01CF   //深蓝色
#define LIGHTBLUE 0X7D7C  //浅蓝色
#define GRAYBLUE 0X5458   //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F  //浅绿色
#define LGRAY 0XC618       //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651  //浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12     //浅棕蓝色(选择条目的反色)

/**
 * LCD Draw 接口
 */

// LCD 相关引脚配置
void LCD_Configure(void);

// LCD 初始化
void LCD_Init(void);

// 开显示
void LCD_DisplayOn(void);

// 关显示
void LCD_DisplayOff(void);

// 清屏
void LCD_Clear(u16 Color);

// 设置光标
void LCD_SetCursor(u16 Xpos, u16 Ypos);

// 画点
void LCD_DrawPoint(u16 x, u16 y);

// 快速画点
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color);

// 读点
u16 LCD_ReadPoint(u16 x, u16 y);

// 画圆
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);

// 画线
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

// 画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);

// 填充单色
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);

// 填充指定颜色
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);

// 填充指定颜色，跳过 clear 颜色
void LCD_Color_Fillx(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color, u16 clear);

// 填充指定颜色，通过 bgcolor 保留背景颜色
// void LCD_Color_Fill2(u16 sx, u16 sy, u16 w, u16 h, u16 *color, u16 *bgcolor,
//                      u16 clear);

// 显示一个字符
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode);

// 显示一个数字
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);

// 显示 数字
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode);

// 显示一个字符串,
// 12/16/24字体
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p);

// LCD printf 配套参数
extern u8 LCD_PRINT_SIZE;
extern u8 LCD_PRINT_MODE;
// LCD printf 输出
void LCD_Draw_printf(u16 x, u16 y, const char *fmt, ...);

// 设置屏扫描方向
void LCD_Scan_Dir(u8 dir);

// 设置屏幕显示方向
void LCD_Display_Dir(u8 dir);

// 设置窗口
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height);

/**
 * LCD 底层交互
 */
void LCD_WriteRegister(u16 LCD_Reg, u16 LCD_RegValue);

u16 LCD_ReadRegister(u16 LCD_Reg);

#endif
