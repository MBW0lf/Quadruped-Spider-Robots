/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
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
/*Setting an initial value for the angle of each motor.*/
int Motor_Current_Position[4][5] = {{0, 0, 0, 0, 0}, {0, 79, 75, 77, 78}, 
{0, 125, 116, 117, 125}, {0, 94, 86, 90, 85}}; 
int space = 0, count = 0;
char symbol;
uint8_t RxBuffer[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int Find_max(int pwm_1, int pwm_2, int pwm_3, int pwm_4);
void Move_All(int pwm_1, int pwm_2, int pwm_3, int pwm_4, int motor);
void Move(TIM_HandleTypeDef htim, int timer, int CCR, int pwm, int motor);
void Home(void);
void Forward(int count);
void Backward(int count);
void Turn_Left(int count);
void Turn_Right(int count);
void deCode(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
////////////////////////////////////////////////////////////////////////////////////
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	////////////////////////////////////////////////////////////// Leg1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //A8	=> M3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);	//B1	=> M2
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); //B9  => M1
	////////////////////////////////////////////////////////////// Leg2
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);	//A1	=> M3
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //A2	=> M2
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);	//A3	=> M1
	////////////////////////////////////////////////////////////// Leg3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //A6	=> M1
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);	//A7	=> M2
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); //B0	=> M3
	////////////////////////////////////////////////////////////// Leg4
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); //B6	=> M1
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);	//B7	=> M2
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //B8	=> M3
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_UART_Receive_DMA(&huart1, RxBuffer, 4);
	Home();
	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(symbol)
		{
			case 'P':
				if(space < 110)
					Forward(1);
				else
				{
					Turn_Right(5);
					Forward(11);
					Turn_Left(5);
					Forward(11);
					Turn_Right(5);
				}
				break;
			case 'B':
				if(space < 105)
					Forward(1);
				else
					Forward(20);
				break;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*Finding the maximum value among the four motors.*/
int Find_max(int pwm_1, int pwm_2, int pwm_3, int pwm_4)
{
	int max = pwm_1;
	if(max < pwm_2)
		max = pwm_2;
	if(max < pwm_3)
		max = pwm_3;
	if(max < pwm_4)
		max = pwm_4;
	return max;
}
////////////////////////////////////////////////////////////////////////////////
/*Simultaneously moving all four motors in a row.*/
void Move_All(int pwm_1, int pwm_2, int pwm_3, int pwm_4, int motor)
{
	int n, max;
	max = Find_max(pwm_1, pwm_2, pwm_3, pwm_4);
	switch(motor)
	{
		case 1:
			n = 50;
			for(;n <= max;n++)
			{
				if(pwm_3 >= n)
					htim3.Instance->CCR1 = n;
				if(pwm_1 >= n)
					htim4.Instance->CCR4 = n;
				if(pwm_4 >= n)
					htim4.Instance->CCR1 = n;
				if(pwm_2 >= n)
					htim2.Instance->CCR4 = n;
				HAL_Delay(10);
			}
			break;
		case 2:
			n = 70;
			for(;n <= max;n++)
			{
				if(pwm_3 >= n)
					htim3.Instance->CCR2 = n;
				if(pwm_1 >= n)
					htim3.Instance->CCR4 = n;
				if(pwm_4 >= n)
					htim4.Instance->CCR2 = n;
				if(pwm_2 >= n)
					htim2.Instance->CCR3 = n;
				HAL_Delay(10);
			}
			break;
		case 3:
			n = 45;
			for(;n <= max;n++)
			{
				if(pwm_3 >= n)
					htim3.Instance->CCR3 = n;
				if(pwm_1 >= n)
					htim1.Instance->CCR1 = n;
				if(pwm_4 >= n)
					htim4.Instance->CCR3 = n;
				if(pwm_2 >= n)
					htim2.Instance->CCR2 = n;
				HAL_Delay(10);
			}
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
/*Moving the motor to a desired angle.*/
void Move(TIM_HandleTypeDef htim, int timer, int CCR, int pwm, int motor)
{
	int n;
	if( timer == 3 && CCR == 3)
		n = Motor_Current_Position[motor][1];
	else if( timer == 4 && CCR == 4)
		n = Motor_Current_Position[motor][1];
	else
		n = Motor_Current_Position[motor][timer];
	if(n <= pwm)
	{
		switch(CCR)
		{
			case 1:
					for(;n <= pwm;n++)
					{
						htim.Instance->CCR1 = n;
						HAL_Delay(10);
					}
					break;
			case 2:
					for(;n <= pwm;n++)
					{
						htim.Instance->CCR2 = n;
						HAL_Delay(10);
					}
					break;
			case 3:
					for(;n <= pwm;n++)
					{
						htim.Instance->CCR3 = n;
						HAL_Delay(10);
					}
					break;
			case 4:
					for(;n <= pwm;n++)
					{
						htim.Instance->CCR4 = n;
						HAL_Delay(10);
					}
					break;
		}
	}
	else
	{
		switch(CCR)
		{
			case 1:
					for(;n >= pwm;n--)
					{
						htim.Instance->CCR1 = n;
						HAL_Delay(10);
					}
					break;
			case 2:
					for(;n >= pwm;n--)
					{
						htim.Instance->CCR2 = n;
						HAL_Delay(10);
					}
					break;
			case 3:
					for(;n >= pwm;n--)
					{
						htim.Instance->CCR3 = n;
						HAL_Delay(10);
					}
					break;
			case 4:
					for(;n >= pwm;n--)
					{
						htim.Instance->CCR4 = n;
						HAL_Delay(10);
					}
					break;
		}
	}
	if( timer == 3 && CCR == 3)
		Motor_Current_Position[motor][1] = pwm;
	else if( timer == 4 && CCR == 4)
		Motor_Current_Position[motor][1] = pwm;
	else
		Motor_Current_Position[motor][timer] = pwm;
}
////////////////////////////////////////////////////////////////////////////////
/*Initial state of the robot in complete stability.*/
void Home()
{
	HAL_Delay(2000);
	Move_All(79, 75, 77, 78, 1);
	Move_All(125, 116, 117, 125, 2);
	Move_All(94, 86, 90, 85, 3); 
	HAL_Delay(1000);
}
////////////////////////////////////////////////////////////////////////////////
/*Moving forward by the desired amount.*/
void Forward(int count)
{
	for(int i = 1;i <= count;i++)
	{
		///////////////////////////
		Move(htim3, 3, 2, 107, 2); 
		Move(htim3, 3, 3, 80, 3);  
		Move(htim4, 4, 1, 103, 1);
		Move(htim3, 3, 2, 117, 2); 
		Move(htim3, 3, 3, 90, 3); 
		Move(htim2, 2, 3, 111, 2); 
		Move(htim2, 2, 2, 76, 3);  
		Move(htim4, 4, 1, 78,1);
		Move(htim2, 2, 3, 116, 2); 
		Move(htim2, 2, 2, 86, 3); 
		HAL_Delay(300);
		///////////////////////////  
		Move(htim2, 2, 3, 106, 2);
		Move(htim2, 2, 2, 76, 3);
		Move(htim4, 4, 4, 54, 1); 	
		Move(htim2, 2, 3, 116, 2);
		Move(htim2, 2, 2, 86, 3);
		Move(htim3, 3, 2, 112, 2); 
		Move(htim3, 3, 3, 85, 3);
		Move(htim4, 4, 4, 79, 1);
		Move(htim3, 3, 2, 117, 2); 
		Move(htim3, 3, 3, 90, 3);
		HAL_Delay(300);
	}
}
////////////////////////////////////////////////////////////////////////////////
/*Moving backward by the desired amount.*/
void Backward(int count)
{
	for(int i = 1;i <= count;i++)
	{
		///////////////////////////
		Move(htim4, 4, 2, 115, 2); 
		Move(htim4, 4, 3, 75, 3);  
		Move(htim3, 3, 1, 102, 1);
		Move(htim4, 4, 2, 125, 2); 
		Move(htim4, 4, 3, 85, 3); 
		Move(htim3, 3, 4, 120, 2); 
		Move(htim1, 1, 1, 89, 3);  
		Move(htim3, 3, 1, 77,1);
		Move(htim3, 3, 4, 125, 2); 
		Move(htim1, 1, 1, 94, 3); 
		HAL_Delay(300);
		///////////////////////////  
		Move(htim3, 3, 4, 115, 2);
		Move(htim1, 1, 1, 84, 3);
		Move(htim2, 2, 4, 50, 1); 	
		Move(htim3, 3, 4, 125, 2);
		Move(htim1, 1, 1, 94, 3);
		Move(htim4, 4, 2, 120, 2); 
		Move(htim4, 4, 3, 80, 3);
		Move(htim2, 2, 4, 75, 1);
		Move(htim4, 3, 2, 125, 2); 
		Move(htim4, 3, 3, 85, 3);
		HAL_Delay(300);
	}
}
////////////////////////////////////////////////////////////////////////////////
/*Turning left by the desired amount.*/
void Turn_Left(int count)
{
	Move(htim2, 2, 3, 111, 2);
	for(int i = 1;i <= count;i++)
	{
		///////////////////////////
		Move(htim4, 4, 2, 110, 2); 
		Move(htim4, 4, 3, 70, 3);  
		Move(htim3, 3, 1, 107, 1);
		Move(htim4, 4, 2, 125, 2); 
		Move(htim4, 4, 3, 85, 3); 
		///////////////////////////
		Move(htim3, 3, 3, 70, 3);
		Move(htim4, 4, 1, 108, 1);
		Move(htim3, 3, 3, 90, 3);
		for(int p = 1;p <= 30;p++)
		{
			htim3.Instance->CCR1 = 107 - p;
			htim4.Instance->CCR1 = 108 - p;
			HAL_Delay(10);
		}
		Motor_Current_Position[1][3] = 77;
		Motor_Current_Position[1][4] = 78;
		HAL_Delay(300);
	}
	Move(htim2, 2, 3, 116, 2);
}
////////////////////////////////////////////////////////////////////////////////
/*Turning right by the desired amount.*/
void Turn_Right(int count)
{
	Move(htim3, 3, 2, 114, 2);
	for(int i = 1;i <= count;i++)
	{
		///////////////////////////
		Move(htim2, 2, 3, 101, 2); 
		Move(htim2, 2, 2, 71, 3);  
		Move(htim4, 4, 4, 49, 1);
		Move(htim2, 2, 3, 116, 2); 
		Move(htim2, 2, 2, 86, 3); 
		///////////////////////////
		Move(htim1, 1, 1, 74, 3);
		Move(htim2, 2, 4, 45, 1);
		Move(htim1, 1, 1, 94, 3);
		for(int p = 1;p <= 30;p++)
		{
			htim4.Instance->CCR4 = 49 + p;
			htim2.Instance->CCR4 = 45 + p;
			HAL_Delay(10);
		}
		Motor_Current_Position[1][1] = 79;
		Motor_Current_Position[1][2] = 75;
		HAL_Delay(300);
	}
	Move(htim3, 3, 2, 117, 2);
}
////////////////////////////////////////////////////////////////////////////////
void deCode()
{
	int index = 5;
	if(RxBuffer[0] == 66 || RxBuffer[0] == 78 || RxBuffer[0] == 80 || RxBuffer[0] == 89) 
	{
		index = 0;
		symbol = RxBuffer[0];
	}
	else if(RxBuffer[1] == 66 || RxBuffer[1] == 78 || RxBuffer[1] == 80 || RxBuffer[1] == 89) 
	{
		index = 1;
		symbol = RxBuffer[1];
	}
	else if(RxBuffer[2] == 66 || RxBuffer[2] == 78 || RxBuffer[2] == 80 || RxBuffer[2] == 89) 
	{
		index = 2;
		symbol = RxBuffer[2];
	}
	else if(RxBuffer[3] == 66 || RxBuffer[3] == 78 || RxBuffer[3] == 80 || RxBuffer[3] == 89)
	{
		index = 3;
		symbol = RxBuffer[3];
	}
	switch(index)
	{
		case 0:
			space = (RxBuffer[3] - 48) + (RxBuffer[2] - 48) * 10 + (RxBuffer[1] - 48) * 100;
			break;
		case 1:
			space = (RxBuffer[0] - 48) + (RxBuffer[3] - 48) * 10 + (RxBuffer[2] - 48) * 100;
			break;
		case 2:
			space = (RxBuffer[1] - 48) + (RxBuffer[0] - 48) * 10 + (RxBuffer[3] - 48) * 100;
			break;
		case 3:
			space = (RxBuffer[2] - 48) + (RxBuffer[1] - 48) * 10 + (RxBuffer[0] - 48) * 100;
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1){
		deCode();
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
