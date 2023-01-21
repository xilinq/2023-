#include "interrupt.h"

keys key[3]={1,0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		
		for(int i=0;i<3;i++)
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
}

