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
#include "adc.h"
#include "tim.h"
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
uint8_t view=0;
uint8_t mode=0;//0自动，1手动
extern struct keys key[4];

uint8_t pa6_duty=10-1;
uint8_t pa7_duty=10-1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void key_proc(void);
void disp_proc(void);
void pwm_proc(void);

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
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,10-1);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,10-1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		key_proc();
		disp_proc();
		pwm_proc();
		
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
		if((view==1)&&(mode==1))
		{
			pa6_duty+=10;
			if(pa6_duty>90-1)
				pa6_duty=10-1;
		}	
		key[1].key_single=0;
	}
	if(key[2].key_single==1)
	{
		if((view==1)&&(mode==1))
		{
			pa7_duty+=10;
			if(pa7_duty>90-1)
				pa7_duty=10-1;			
		}
		key[2].key_single=0;
	}
	if(key[3].key_single==1)
	{
		mode=!mode;
		key[3].key_single=0;
	}
}

void disp_proc()
{
	if(view==0)
	{
		char text[20];
		sprintf(text,"      Data   ");
		LCD_DisplayStringLine(Line0, (uint8_t *)text);
		sprintf(text,"    V:%.2fV      ",get_adc(&hadc2));
		LCD_DisplayStringLine(Line2, (uint8_t *)text);
		if(mode==0)
		{
			sprintf(text,"    Mode:AUTO     ");
			LCD_DisplayStringLine(Line4, (uint8_t *)text);
		}
		if(mode==1)
		{
			sprintf(text,"    Mode:MANU     ");
			LCD_DisplayStringLine(Line4, (uint8_t *)text);
		}
		sprintf(text,"  AUTO PA:%d%%   ",(int)(get_adc(&hadc2)/3.3*100)/1);
		LCD_DisplayStringLine(Line6, (uint8_t *)text);
	}
	if(view==1)
	{
		char text[20];
		sprintf(text,"      Para   ");
		LCD_DisplayStringLine(Line0, (uint8_t *)text);
		sprintf(text,"    PA6:%d%%   ",pa6_duty+1);
		LCD_DisplayStringLine(Line2, (uint8_t *)text);
		sprintf(text,"    PA7:%d%%   ",pa7_duty+1);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
	}
}

void pwm_proc()
{
	if(mode==1)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,pa6_duty);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,pa7_duty);
	}
	if(mode==0)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,(int)(get_adc(&hadc2)/3.3*100)/1);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,(int)(get_adc(&hadc2)/3.3*100)/1);
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
