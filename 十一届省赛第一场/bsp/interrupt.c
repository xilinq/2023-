#include "interrupt.h"
#include "adc.h"
#include "led.h"
struct keys key[4]={1,0,0,0,0};
uint8_t	adc_time=0;
uint8_t adc_time_s=0;
float adc_value=0;
extern float vmax,vmin;
uint8_t adc_sta=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
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
					{
						key[i].judge_sta=1;
						key[i].key_time=0;
					}
				};break;
				case 1:
				{
					if(key[i].key_sta==0)
						key[i].judge_sta=2;
					else
						key[i].judge_sta=0;
				};break;
				case 2:
				{
					if(key[i].key_sta==1)
					{
						key[i].judge_sta=0;
						if(key[i].key_time<70)
							key[i].key_single=1;
					}
					else
					{
						key[i].key_time++;
						if(key[i].key_time>=70)
							key[i].key_long=1;
					}
				};break;
			}
		}
	}
	if(htim->Instance == TIM4)
	{
		adc_value=get_adc(&hadc2);		
		switch(adc_sta)
		{
			case 0:
			{				
				if(adc_value<vmin)
					adc_sta=0;
				else
				{
					adc_time_s=0;
					adc_sta=1;
					adc_time=1;
				}				
			};break;
			case 1:
			{
				if(adc_value>vmax)
					adc_sta=2;	
				else if(adc_value<vmin)
					adc_sta=0;
				else
				{
					adc_time++;
					if(adc_time==100)
					{
						adc_time_s++;
						adc_time=0;
					}
				}	
			};break;
			case 2:
			{
				if(adc_value<vmin)
					adc_sta=0;
			};break;
		}		
	}
	
}
