/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern struct keys key[4];
bool view=0;
bool pwm_sta=0;

float cnbr=3.50;
float vnbr=2.00;

uint8_t cnbr_num=0;
uint8_t vnbr_num=0;

extern char rxdata[30];
extern uint8_t rxbuf;
extern uint8_t rxpointer;

char car_type[5];
char car_name[5];
uint8_t Y,M,D,h,m,s;

uint32_t car_time;
uint32_t now_time;
uint32_t difference_time;

cars car[8];
bool ok_flag=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void key_proc(void);
void disp_proc(void);
void pwm_proc(void);
void rx_proc(void);
void inputdata_proc(void);
void led_proc(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_UART_Receive_IT(&huart1,&rxbuf,1);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
//	led_init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		key_proc();
		disp_proc();
		pwm_proc();		
		led_proc();

		if(rxpointer!=0)
		{
			uint8_t temp;
			temp=rxpointer;
			HAL_Delay(1);
			if(temp==rxpointer)
				rx_proc();
		}
	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void key_proc()
{
	if(key[0].key_single==1)
	{
		view=!view;
		key[0].key_single=0;
	}
	if(key[1].key_single==1)
	{
		if(view==1)
		{
			cnbr+=0.5;
			vnbr+=0.5;
		}
		key[1].key_single=0;
	}
	if(key[2].key_single==1)
	{
		if(view==1)
		{
			cnbr-=0.5;
			if(cnbr<0.00)
				cnbr=0;
			vnbr-=0.5;
			if(vnbr<0.00)
				vnbr=0;
		}
		key[2].key_single=0;
	}
	if(key[3].key_single==1)
	{
		pwm_sta=!pwm_sta;
		key[3].key_single=0;
	}
}

void disp_proc()
{
	if(view==0)
	{		
		char text[20];
		sprintf(text,"       Data    ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text,"   CNBR:%d    ",cnbr_num);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"   VNBR:%d    ",vnbr_num);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
		sprintf(text,"   IDLE:%d    ",(8-cnbr_num-vnbr_num));
		LCD_DisplayStringLine(Line7, (uint8_t *)text);
	}
	if(view==1)
	{		
		char text[20];
		sprintf(text,"       Para    ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text,"   CNBR:%.2f    ",cnbr);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"   VNBR:%.2f    ",vnbr);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
		sprintf(text,"            ");
		LCD_DisplayStringLine(Line7, (uint8_t *)text);
	}
}

void pwm_proc()
{
	if(pwm_sta==0)
	{
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	}
	if(pwm_sta==1)
	{
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,20);
	}
}

void rx_proc()
{
	if(rxpointer>0)
	{
		
//判断读入数据正确性
		if(rxpointer==22)//输入长度正确
		{
//			sscanf(rxdata,"%4s:%4s:%12s",car_type,car_name,&car_time);		
//			car_time_proc();
//			char text[30];
//			sprintf(text,"%s\r\n",&car_time);
//			HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
			
			if(((rxdata[0]=='C')||(rxdata[0]=='V'))&&(rxdata[1]=='N')&&(rxdata[2]=='B')&&(rxdata[3]=='R')&&(rxdata[4]==':')&&(rxdata[9]==':'))//判断输入格式正确
			{
				for(int i=0;i<4;i++)
				{
					car_type[i]=rxdata[i];
				}
				car_type[4]='\0';
				for(int i=0;i<4;i++)
				{
					car_name[i]=rxdata[i+5];
				}
				car_name[4]='\0';
				
				Y=(rxdata[10]-'0')*10+(rxdata[11]-'0');
				M=(rxdata[12]-'0')*10+(rxdata[13]-'0');
				D=(rxdata[14]-'0')*10+(rxdata[15]-'0');
				h=(rxdata[16]-'0')*10+(rxdata[17]-'0');
				m=(rxdata[18]-'0')*10+(rxdata[19]-'0');
				s=(rxdata[20]-'0')*10+(rxdata[21]-'0');
				
//				char text[30];
//				sprintf(text,"%s\r\n",car_type);
//				HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
//				sprintf(text,"%s\r\n",car_name);
//				HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
//				sprintf(text,"%d,%d,%d,%d,%d,%d\r\n",Y,M,D,h,m,s);
//				HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
				
				if((M>12)||(D>31)||(h>23)||(m>59)||(s>59))//判断输入数据正确
				{
					char text[30];
					sprintf(text,"ERROR\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
				}
				else //判断完毕
				{
//					char text[30];
//					sprintf(text,"%s\r\n",car_name);
//					HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					inputdata_proc();
				
				}
			}
			else 
			{
				char text[30];
				sprintf(text,"ERROR\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
			}
			
		}
		else
		{
			char text[30];
			sprintf(text,"ERROR\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
		}
		rxpointer=0;
		memset(rxdata,0,30);
	}
}

void inputdata_proc()
{
//查询是否已经有此车
	ok_flag=0;
	for(int i=0;i<8;i++)
	{
		if(car[i].isfull)//与库中车做对比
		{			
			if((car_name[0]==car[i].car_name[0])&&(car_name[1]==car[i].car_name[1])&&(car_name[2]==car[i].car_name[2])&&(car_name[3]==car[i].car_name[3]))	
			{
			//库中已经有此车
				if(car_type[0]=='C')
					cnbr_num--;
				if(car_type[0]=='V')
					vnbr_num--;
				
			//计算时间差和费用
				if(M<car[i].mon)
				{
					char text[30];
					sprintf(text,"wrong time\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
				}
				else 
				{
					if(M==car[i].mon)
					{
						car_time=car[i].day*3600*24+car[i].hour*3600+car[i].min*60+car[i].sec;
						now_time=D*3600*24+h*3600+m*60+s;						
					}
					else
					{
						if(M-car[i].mon>1)
						{
							char text[30];
							sprintf(text,"too long time\r\n");
							HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
						}
						else
						{
							if(M-car[i].mon==1)
							{
								car_time=car[i].day*3600*24+car[i].hour*3600+car[i].min*60+car[i].sec;
								switch(car[i].mon)
								{
									case 1:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;			
									}
									case 2:
									{
										now_time=3600*24*28+D*3600*24+h*3600+m*60+s;
									}
									case 3:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									case 4:
									{
										now_time=3600*24*30+D*3600*24+h*3600+m*60+s;
									}
									case 5:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									case 6:
									{
										now_time=3600*24*30+D*3600*24+h*3600+m*60+s;
									}
									case 7:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									case 8:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									case 9:
									{
										now_time=3600*24*30+D*3600*24+h*3600+m*60+s;
									}
									case 10:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									case 11:
									{
										now_time=3600*24*30+D*3600*24+h*3600+m*60+s;
									}
									case 12:
									{
										now_time=3600*24*31+D*3600*24+h*3600+m*60+s;
									}
									
								}
							}
						}
					}
				}

				difference_time=now_time-car_time;
				if(difference_time%3600==0)
				{
					if(car_type[0]=='C')
					{
						char text[30];
						sprintf(text,"%s:%s:%d:%.2f\r\n",car_type,car_name,difference_time/3600,cnbr*difference_time/3600);
						HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					}

					if(car_type[0]=='V')
					{
						char text[30];
						sprintf(text,"%s:%s:%d:%.2f\r\n",car_type,car_name,difference_time/3600,vnbr*difference_time/3600);
						HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					}
				}
				else
				{
					if(car_type[0]=='C')
					{
						char text[30];
						sprintf(text,"%s:%s:%d:%.2f\r\n",car_type,car_name,(difference_time/3600)+1,cnbr*((difference_time/3600)+1));
						HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					}

					if(car_type[0]=='V')
					{
						char text[30];
						sprintf(text,"%s:%s:%d:%.2f\r\n",car_type,car_name,(difference_time/3600)+1,vnbr*((difference_time/3600)+1));
						HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					}
				}
				
//				char text[30];
//				sprintf(text,"already had\r\n");
//				HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
				
				car[i].isfull=0;
				ok_flag=1;
				break;
			}
		}
	}
	if(ok_flag==0)//库中没有此车
	{
		if((cnbr_num+vnbr_num)==8)//车位已满，停不下了
		{
			char text[30];
			sprintf(text,"NO PLACE!\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
		}
		else
		{
			//还有车位，寻找空车位
			for(int i=0;i<8;i++)
			{
				if(car[i].isfull==0)//找到空车位
				{
					if(car_type[0]=='C')
						cnbr_num++;
					if(car_type[0]=='V')
						vnbr_num++;
					for(int j=0;j<4;j++)//读入数据
					{
						car[i].car_name[j]=car_name[j];
						car[i].car_type[j]=car_type[j];
					}
					//读入时间
					car[i].year=Y;
					car[i].mon=M;
					car[i].day=D;
					car[i].hour=h;
					car[i].min=m;
					car[i].sec=s;
					car[i].isfull=1;
					
					char text[30];
					sprintf(text,"input ok\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t *)text,strlen(text),50);
					
					break;
				}
			}		
		}		
	}	
}

void led_proc()
{
	led_init();
	if((8-cnbr_num-vnbr_num)==0)
		LD1_off();
	else
		LD1_on();
	if(pwm_sta==0)
		LD2_off();
	else
		LD2_on();
	
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
