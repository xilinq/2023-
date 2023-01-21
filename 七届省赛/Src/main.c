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

uint8_t th1=30;
uint8_t th2=50;
uint8_t th3=70;
uint8_t th1_temp;
uint8_t th2_temp;
uint8_t th3_temp;
uint8_t whichth=0;

uint8_t level=0;
float adc_value=0;
uint8_t height=0;
uint8_t level_temp;
uint8_t level_flag=0;

extern char rxdata[30];
extern uint8_t rxtemp;
extern uint8_t rxpointer;

extern uint16_t count;
bool ld2_flag=0;
extern uint16_t ld2_cnt;
extern uint8_t ld2_cntt;

bool chaxun_flag=0;
bool ld3_flag=0;
extern uint16_t ld3_cnt;
extern uint8_t ld3_cntt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void disp_proc(void);
void key_proc(void);
void rx_proc(void);
void tx_proc(void);
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
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_UART_Receive_IT(&huart1, &rxtemp, 1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	th1=eeprom_read(0x01);
	th2=eeprom_read(0x02);
	th3=eeprom_read(0x03);

//
//	HAL_ADC_Start(&hadc2);
	for(int i=0;i<20;i++)
		adc_value=get_adc(&hadc2);	
	height=(int)(adc_value/3.3*100)/1;
	if(height<=th1)
			level_temp=0;
		else if((height>th1)&&(height<=th2))
			level_temp=1;
		else if((height>th2)&&(height<=th3))
			level_temp=2;
		else if(height>th3)
			level_temp=3;
//

//	led_init();	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  disp_proc();
		led_proc();
		tx_proc();		
		key_proc();
		
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
void disp_proc()
{
	if(view==0)
	{
		adc_value=get_adc(&hadc2);
		char text[20];
		sprintf(text,"     Liquid Level      ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		height=(int)(adc_value/3.3*100)/1;
		sprintf(text,"   Height:%dcm        ",height);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"   ADC:%.2fV           ",adc_value);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		if(height<=th1)
			level=0;
		else if((height>th1)&&(height<=th2))
			level=1;
		else if((height>th2)&&(height<=th3))
			level=2;
		else if(height>th3)
			level=3;
		if(level_temp>level)//down
			level_flag=1;
		else if(level_temp<level)//up
			level_flag=2;
		level_temp=level;
		sprintf(text,"   Level:%d         ",level);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
	if(view==1)
	{
		char text[20];
		sprintf(text,"     Parameter Setup      ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		switch(whichth)
		{
			case 0:
			{
				LCD_SetTextColor(Green);
				sprintf(text,"   Threshold 1:%d         ",th1_temp);
				LCD_DisplayStringLine(Line3, (uint8_t *)text);
				LCD_SetTextColor(White);
				sprintf(text,"   Threshold 2:%d         ",th2_temp);
				LCD_DisplayStringLine(Line4, (uint8_t *)text);
				sprintf(text,"   Threshold 3:%d         ",th3_temp);
				LCD_DisplayStringLine(Line5, (uint8_t *)text);			
			};break;
			case 1:
			{
				sprintf(text,"   Threshold 1:%d         ",th1_temp);
				LCD_DisplayStringLine(Line3, (uint8_t *)text);
				LCD_SetTextColor(Green);
				sprintf(text,"   Threshold 2:%d         ",th2_temp);
				LCD_DisplayStringLine(Line4, (uint8_t *)text);
				LCD_SetTextColor(White);
				sprintf(text,"   Threshold 3:%d         ",th3_temp);
				LCD_DisplayStringLine(Line5, (uint8_t *)text);
			};break;
			case 2:
			{
				sprintf(text,"   Threshold 1:%d         ",th1_temp);
				LCD_DisplayStringLine(Line3, (uint8_t *)text);
				sprintf(text,"   Threshold 2:%d         ",th2_temp);
				LCD_DisplayStringLine(Line4, (uint8_t *)text);
				LCD_SetTextColor(Green);
				sprintf(text,"   Threshold 3:%d         ",th3_temp);
				LCD_DisplayStringLine(Line5, (uint8_t *)text);
				LCD_SetTextColor(White);
			};break;
		}	
	}
}

void key_proc()
{
	if(key[0].key_single==1)
	{
		if(view==0)
		{
			whichth=0;
			th1_temp=th1;
			th2_temp=th2;
			th3_temp=th3;
			view=1;
		}
		else
		{
			th1=th1_temp;
			th2=th2_temp;
			th3=th3_temp;
			eeprom_write(0x01,th1);
			HAL_Delay(10);
			eeprom_write(0x02,th2);
			HAL_Delay(10);
			eeprom_write(0x03,th3);
			HAL_Delay(10);
			view=0;
		}
		key[0].key_single=0;
	}
	if(key[1].key_single==1)
	{
		if(view==1)
		{
			if(++whichth==3)
				whichth=0;
		}
		key[1].key_single=0;
	}
	if(key[2].key_single==1)
	{
		if(view==1)
		{
			switch(whichth)
			{
				case 0:
				{
					th1_temp+=5;
					if(th1_temp>95)
						th1_temp=95;
				};break;
				case 1:
				{
					th2_temp+=5;
					if(th2_temp>95)
						th2_temp=95;
				};break;
				case 2:
				{
					th3_temp+=5;
					if(th3_temp>95)
						th3_temp=95;
				};break;
			}
		}
		key[2].key_single=0;
	}
	if(key[3].key_single==1)
	{
		if(view==1)
		{
			switch(whichth)
			{
				case 0:
				{
					th1_temp-=5;
					if(th1_temp<5)
						th1_temp=5;
				};break;
				case 1:
				{
					th2_temp-=5;
					if(th2_temp<5)
						th2_temp=5;
				};break;
				case 2:
				{
					th3_temp-=5;
					if(th3_temp<5)
						th3_temp=5;
				};break;
			}
		}
		key[3].key_single=0;
	}
}

void rx_proc()
{
	if(rxpointer==1)
	{
		if(rxdata[0]=='C')
		{
			char text[30];
			sprintf(text,"C:H%d+L%d\r\n",height,level);
			HAL_UART_Transmit(&huart1, (uint8_t *)text, strlen(text), 50);
			chaxun_flag=1;
		}
		if(rxdata[0]=='S')
		{
			char text[30];
			sprintf(text,"S:TL%d+TM%d+TH%d\r\n",th1,th2,th3);
			HAL_UART_Transmit(&huart1, (uint8_t *)text, strlen(text), 50);
			chaxun_flag=1;
		}
	}
	rxpointer=0;
	memset(rxdata,0,30);
}

void tx_proc()
{
	if(level_flag==1)
	{
		char text[30];
		sprintf(text,"A:H%d+L%d+D\r\n",height,level);
		HAL_UART_Transmit(&huart1, (uint8_t *)text, strlen(text), 50);				
	}
	else if(level_flag==2)
	{
		char text[30];
		sprintf(text,"A:H%d+L%d+U\r\n",height,level);
		HAL_UART_Transmit(&huart1, (uint8_t *)text, strlen(text), 50);	
	}
	level_flag=0;
}

void led_proc()
{
	led_init();
	ld1(count>=1000);
	if(level_flag!=0)
	{
		ld2_cnt=0;
		ld2_cntt=0;
		ld2_flag=1;	
	}
	ld2(ld2_cnt>=200);
	if(chaxun_flag==1)
	{
		ld3_cnt=0;
		ld3_cntt=0;
		ld3_flag=1;
		chaxun_flag=0;
	}
	ld3(ld3_cnt>=200);
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
