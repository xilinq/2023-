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
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
RTC_TimeTypeDef H_M_S_Time;
RTC_DateTypeDef Y_M_D_Date;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool view=0;
extern keys key[4];

float k=0.1;

extern char rxdata[30];
extern uint8_t rxtemp;
extern uint8_t rxpointer;

float k_new=0;

uint16_t k_8=0;

time_sets timeset={0,0,0};
time_sets timeset0={0,0,0};

bool is_led_open=1;
bool led_flag=0;

uint8_t whitch_para=0;

bool send_flag=0;
extern uint16_t cnt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void disp_proc(void);
void key_proc(void);
void rx_proc(void);
void adc_send(void);
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
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
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
		
	k_8=eeprom_read(0x02)+((uint16_t)(eeprom_read(0x01))<<8);	
	k=(float)k_8/1000;
		
	led_init();	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_RTC_GetTime(&hrtc,&H_M_S_Time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&Y_M_D_Date,RTC_FORMAT_BIN);		
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
		adc_send();
		led_proc();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
		sprintf(text,"   V1:%.2fV        ",get_adc(&hadc2));
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text,"   k:%.1f        ",k);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		if(is_led_open==0)
			sprintf(text,"   LED:OFF        ");
		if(is_led_open==1)
			sprintf(text,"   LED:ON         ");
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text,"   T:%d%d-%d%d-%d%d         ",H_M_S_Time.Hours/10,H_M_S_Time.Hours%10,H_M_S_Time.Minutes/10,H_M_S_Time.Minutes%10,H_M_S_Time.Seconds/10,H_M_S_Time.Seconds%10);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
	if(view==1)
	{
		char text[20];
		sprintf(text,"      SETTING   ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		switch(whitch_para)
		{
			case 0:
			{
				if(led_flag==0)
					sprintf(text,"    %d%d-%d%d-%d%d    ",timeset0.hours/10,timeset0.hours%10,timeset0.minutes/10,timeset0.minutes%10,timeset0.seconds/10,timeset0.seconds%10);
				else
					sprintf(text,"      -%d%d-%d%d       ",timeset0.minutes/10,timeset0.minutes%10,timeset0.seconds/10,timeset0.seconds%10);
			};break;
			case 1:
			{
				if(led_flag==0)
					sprintf(text,"    %d%d-%d%d-%d%d    ",timeset0.hours/10,timeset0.hours%10,timeset0.minutes/10,timeset0.minutes%10,timeset0.seconds/10,timeset0.seconds%10);
				else
					sprintf(text,"    %d%d-  -%d%d        ",timeset0.hours/10,timeset0.hours%10,timeset0.seconds/10,timeset0.seconds%10);
			};break;
			case 2:
			{
				if(led_flag==0)
					sprintf(text,"    %d%d-%d%d-%d%d    ",timeset0.hours/10,timeset0.hours%10,timeset0.minutes/10,timeset0.minutes%10,timeset0.seconds/10,timeset0.seconds%10);
				else
					sprintf(text,"    %d%d-%d%d-        ",timeset0.hours/10,timeset0.hours%10,timeset0.minutes/10,timeset0.minutes%10);
			};break;
		}		
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"                ");
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text,"                ");
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
}

void key_proc()
{
	if(key[0].key_single==1)
	{
		if(view==0)			
			is_led_open=!is_led_open;
		key[0].key_single=0;
	}
	if(key[1].key_single==1)
	{
		whitch_para=0;		
		if(view==0)
		{
			timeset0.hours=timeset.hours;
			timeset0.minutes=timeset.minutes;
			timeset0.seconds=timeset.seconds;		
			view=1;
		}			
		else
		{
			timeset.hours=timeset0.hours;
			timeset.minutes=timeset0.minutes;
			timeset.seconds=timeset0.seconds;
			view=0;
		}		
		key[1].key_single=0;
	}
	if(key[2].key_single==1)
	{
		if(view==1)
		{
			if(++whitch_para>2)
				whitch_para=0;
		}
		key[2].key_single=0;
	}
	if(key[3].key_single==1)
	{
		if(view==1)
		{
			switch(whitch_para)
			{
				case 0:
				{
					if(++timeset0.hours==24)
						timeset0.hours=0;
				};break;
				case 1:
				{
					if(++timeset0.minutes==60)
						timeset0.minutes=0;
				};break;
				case 2:
				{
					if(++timeset0.seconds==60)
						timeset0.seconds=0;
				};break;
			}
		}
		key[3].key_single=0;
	}
}

void rx_proc()
{
	if((rxdata[0]=='k')&&(rxdata[1]=='0')&&(rxdata[2]=='.')&&(rxdata[rxpointer-1]=='n')&&(rxdata[rxpointer-2]=='\\'))
	{
		k_new=0;
		for(int i=3;i<rxpointer-2;i++)
		{
			if((rxdata[i]<'0')||(rxdata[i]>'9'))
				goto HERE;
			float temp=rxdata[i]-'0';			
			for(int j=1;j<i-1;j++)
			{
				temp/=10;			
			}
			k_new+=temp;
		}
		k=k_new;
		k_8=(uint16_t)(k*1000)/1;
		eeprom_write(0x01,k_8>>8);
		HAL_Delay(10);
		eeprom_write(0x02,k_8&0xff);
		HAL_Delay(10);
		char text[30];
		sprintf(text,"ok\n");
		HAL_UART_Transmit(&huart1,(uint8_t *)&text, strlen(text), 50);
	}	
	HERE:
	rxpointer=0;
	memset(rxdata,0,30);
}

void adc_send()
{
	if((H_M_S_Time.Hours==timeset.hours)&&(H_M_S_Time.Minutes==timeset.minutes)&&(H_M_S_Time.Seconds==timeset.seconds))
	{
		if(send_flag==1)
		{
			char text[30];
			sprintf(text,"%.2f+%.1f+%d%d%d%d%d%d\n",get_adc(&hadc2),k,H_M_S_Time.Hours/10,H_M_S_Time.Hours%10,H_M_S_Time.Minutes/10,H_M_S_Time.Minutes%10,H_M_S_Time.Seconds/10,H_M_S_Time.Seconds%10);
			HAL_UART_Transmit(&huart1,(uint8_t *)&text, strlen(text), 50);
			cnt=0;
			send_flag=0;
		}		
	}
}

void led_proc()
{
	if(is_led_open==1)
	{
		if(get_adc(&hadc2)>(3.3*k))			
			ld1(led_flag);
		else 
			ld1(1);
	}
	else 
		ld1(1);
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
