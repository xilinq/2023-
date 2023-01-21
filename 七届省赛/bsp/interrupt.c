#include "interrupt.h"

keys key[4]={1,0,0};

uint16_t count=0;
extern bool ld2_flag;
uint16_t ld2_cnt=400;
uint8_t ld2_cntt=0;
extern bool ld3_flag;
uint16_t ld3_cnt=400;
uint8_t ld3_cntt=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		for(int i=0;i<4;i++)
		{
			switch(key[i].judge_sta)
			{
				case 0:
				{
					if(key[i].key_sta==0)
						key[i].judge_sta=1;
				};break;
				case 1:
				{
					if(key[i].key_sta==0)
					{
						key[i].key_single=1;
						key[i].judge_sta=2;
					}
					else
						key[i].judge_sta=0;
				};break;
				case 2:
				{
					if(key[i].key_sta==1)
						key[i].judge_sta=0;
				};break;
			}
		}
	}
	if(htim->Instance==TIM4)
	{
		if(++count==2000)
		{
			count=0;
		}
		if(ld2_flag==1)
		{
			if(++ld2_cnt==400)
			{
				ld2_cnt=0;
				ld2_cntt++;
			}			
			if(ld2_cntt==5)
			{
				ld2_flag=0;
				ld2_cnt=400;
				ld2_cntt=0;
			}				
		}
		if(ld3_flag==1)
		{
			if(++ld3_cnt==400)
			{
				ld3_cnt=0;
				ld3_cntt++;
			}
			if(ld3_cntt==5)
			{
				ld3_flag=0;
				ld3_cnt=400;
				ld3_cntt=0;
			}
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
		HAL_UART_Receive_IT(huart, &rxtemp, 1);
	}
}

