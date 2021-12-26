#ifndef __CORE_H
#define __CORE_H

#include <stm32f10x.h>

/**
 * CORE
 */
void SetSysClockTo72(void);
void CORE_Configure(void);

// 启动时间，毫秒
extern u32 core_uptime;
// 毫秒嘀嗒
extern u16 ms_tick;

#endif