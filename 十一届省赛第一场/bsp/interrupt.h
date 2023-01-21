#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "main.h"
#include "stdbool.h"
struct keys
{
	bool key_sta;
	uint8_t judge_sta;
	bool key_single;
	uint8_t key_time;
	bool key_long;
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
