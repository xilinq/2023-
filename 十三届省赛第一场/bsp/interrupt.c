#include "interrupt.h"

keys key[4]={1,0,0};
uint16_t count=0;
uint16_t count1=0;
bool ok_5s=0;
bool ok_5s1=0;
bool ok_01s=0;
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
		if(++count==5000)
		{
			count=0;
			ok_5s=1;
		}
	}
	if(htim->Instance==TIM15)
	{
		if(++count1==5000)
		{
			count1=0;
			ok_5s1=1;
			ok_01s=0;
		}
		if(count1%100==0)
			ok_01s=!ok_01s;
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


