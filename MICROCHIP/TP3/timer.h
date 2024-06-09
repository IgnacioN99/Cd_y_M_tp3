/*
 * timer.h
 *
 * Created: 16/5/2022 15:40:18
 *  Author: Lenovo
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "utils.h"
void TIMER_init(void);
void TIMER_Enable(void);
void TIMER_Disable(void);
uint8_t TIMER_GetClkFlag(void);
void TIMER_ClearClkFlag();
void TIMER_ClearCount();
#endif /* TIMER_H_ */