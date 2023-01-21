#include "interrupt.h"

uint16_t ccr1=0;
uint16_t freq=0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM16)
	{
		if(htim->Channel==1)
		{
			ccr1=__HAL_TIM_GetCounter(htim);
			freq=(80000000/80)/ccr1;
			__HAL_TIM_SetCounter(htim,0);
		}
	}
}


char rxdata[30];
uint8_t rxtemp;
uint8_t rxpointer;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		rxdata[rxpointer++]=rxtemp;
		HAL_UART_Receive_IT(huart,&rxtemp,1);
	}
}


uint16_t count=0;
bool ld1_flag=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		if(ld1_flag==1)
		{
			if(++count==300)
			{
				count=0;
				ld1_flag=0;
			}			
		}
		else count=0;	
	}
}

