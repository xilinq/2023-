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
extern keys key[4];
bool view=0;

char code1[2]={'@'};
char code2[2]={'@'};
char code3[2]={'@'};

char cod1[2]={'1'};
char cod2[2]={'2'};
char cod3[2]={'3'};

extern char rxdata[30];
extern uint8_t rxtemp;
extern uint8_t rxpointer;

extern bool ok_5s;
uint8_t view1_sta=0;

uint8_t wrong_times=0;
uint8_t ld2_sta=0;
extern bool ok_01s;
bool B3_down=0;

extern uint16_t count;
extern bool ok_5s1;
extern uint16_t count1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void disp_proc(void);
void key_proc(void);
void rx_proc(void);
void view1_proc(void);
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
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_UART_Receive_IT(&huart1, &rxtemp, 1);
	
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim15);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	

	led_init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		disp_proc();
		key_proc();
		
		if(rxpointer!=0)
		{
			uint8_t temp;
			temp=rxpointer;
			HAL_Delay(1);
			if(temp==rxpointer)
				rx_proc();
		}
		
		led_proc();
		view1_proc();
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
void disp_proc()
{
	if(view==0)
	{
		char text[20];
		sprintf(text,"       PSD      ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text,"    B1:%s      ",code1);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"    B2:%s      ",code2);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text,"    B3:%s      ",code3);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
	if(view==1)
	{
		char text[20];
		sprintf(text,"       STA      ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text,"    F:2kHz      ");
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"    D:10%%      ");
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text,"                    ");
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
}

void key_proc()
{
	if(key[0].key_single==1)
	{
		if(view==0)
		{
			if(code1[0]=='@')
				code1[0]='0';
			else
			{
				if(++code1[0]>'9')
					code1[0]='0';
			}		
		}		
		key[0].key_single=0;
	}
	if(key[1].key_single==1)
	{
		if(view==0)
		{
			if(code2[0]=='@')
				code2[0]='0';
			else
			{
				if(++code2[0]>'9')
					code2[0]='0';
			}		
		}		
		key[1].key_single=0;
	}
	if(key[2].key_single==1)
	{
		if(view==0)
		{
			if(code3[0]=='@')
				code3[0]='0';
			else
			{
				if(++code3[0]>'9')
					code3[0]='0';
			}		
		}		
		key[2].key_single=0;
	}
	if(key[3].key_single==1)
	{		
		if((code1[0]==cod1[0])&&(code2[0]==cod2[0])&&(code3[0]==cod3[0]))
		{
			view=1;	
			wrong_times=0;
		}			
		else
		{
			B3_down=1;
			wrong_times++;
		}			
	  code3[0]='@';
		code2[0]='@';
		code1[0]='@';		
		key[3].key_single=0;
	}
}

void rx_proc()
{
	if(rxpointer==7)
	{
		if((rxdata[3]=='-')&&(rxdata[0]==cod1[0])&&(rxdata[1]==cod2[0])&&(rxdata[2]==cod3[0])&&((rxdata[4]>='0')&&(rxdata[4]<='9'))&&((rxdata[5]>='0')&&(rxdata[5]<='9'))&&((rxdata[6]>='0')&&(rxdata[6]<='9')))
		{
			cod1[0]=rxdata[4];
			cod2[0]=rxdata[5];
			cod3[0]=rxdata[6];
		}
	}
	rxpointer=0;
	memset(rxdata,0,30);
}

void view1_proc()
{
	switch(view1_sta)
	{
		case 0:
		{
			if(view==1)
			{
				ok_5s=0;
				count=0;
				__HAL_TIM_SetClockDivision(&htim2,400-1);
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,10);				
				ld1(0);				
				view1_sta=1;
			}
		};break;
		case 1:
		{
			if(ok_5s==1)
			{
				view=0;
				ok_5s=0;
				__HAL_TIM_SetClockDivision(&htim2,800-1);
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,50);	
				ld1(1);
				view1_sta=0;
			}
		};break;
	}
}

void led_proc()
{	
	switch(ld2_sta)
	{
		case 0:
		{
			if(B3_down==1)
			{
				if(wrong_times>=3)
				{				
					ok_5s1=0;
					count1=0;
					ld2(0);					
					ld2_sta=1;
				}
				B3_down=0;
			}			
		};break;
		case 1:
		{
			if(ok_5s1==1)
			{
				ld2(1);
				ld2_sta=0;
				ok_5s1=0;
			}
			else
				ld2(ok_01s);
		};break;
	}	
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
