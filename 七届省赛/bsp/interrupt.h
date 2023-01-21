#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "main.h"
#include "stdbool.h"

typedef struct
{
	bool key_sta;
	uint8_t judge_sta;
	bool key_single;
} keys;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif

