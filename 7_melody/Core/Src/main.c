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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NOTE_C3 131
#define NOTE_C3_S 138
#define NOTE_D3 147
#define NOTE_D3_S 155
#define NOTE_E3 165
#define NOTE_F3 174
#define NOTE_F3_S 185
#define NOTE_G3 196
#define NOTE_G3_S 207
#define NOTE_A3 220
#define NOTE_A3_S 233
#define NOTE_H3 247
#define NOTE_C4 261
#define NOTE_C4_S 277
#define NOTE_D4 293
#define NOTE_D4_S 311
#define NOTE_E4 329
#define NOTE_F4 349
#define NOTE_F4_S 370
#define NOTE_G4 392
#define NOTE_G4_S 415
#define NOTE_A4 440
#define NOTE_A4_S 466
#define NOTE_H4 494
#define NOTE_C5 523
#define NOTE_C5_S 554
#define NOTE_D5 587
#define NOTE_D5_S 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_F5_S 740
#define NOTE_G5 784
#define NOTE_G5_S 830
#define NOTE_A5 880
#define NOTE_A5_S 932
#define NOTE_H5 988


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int clock = 32000000;
int frequency;
int ACC;
int CCR;
int note;
int rythm;
int tempo;
int wholenote;
int divide;
int duration;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void frequency_change(int note) {
  ACC = clock/note;
  CCR = ACC/2;
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_AUTORELOAD(&htim2, ACC);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, CCR);
};

void play_note(int rythm) {
  wholenote = 240000 / tempo;
  if(rythm <= 16 && rythm >= 0){
    duration = wholenote / rythm;
  }else if(rythm < 0){
    duration = wholenote / abs(rythm);
    duration *= 1.5;
  }else{
    duration = wholenote * 10 / rythm;
    duration *= 0.5;
  }
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_Delay(duration*0.9);
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
  HAL_Delay(duration);
};

void play(int note, int rythm){
	frequency_change(note);
	play_note(rythm);
};

void godfather(){
  tempo = 100;

  play(NOTE_E4, 8);
	play(NOTE_A4, 8);
	play(NOTE_C5, 8);
  play(NOTE_H4, 8);
	play(NOTE_A4, 8);
	play(NOTE_C5, 8);
  play(NOTE_A4, 8);
  play(NOTE_H4, 8);
  play(NOTE_A4, 8);
  play(NOTE_F4, 8);
  play(NOTE_G4, 8);
  play(NOTE_E4, 8);
  play(0, 6);
  play(NOTE_E4, 8);
  play(NOTE_A4, 8);
  play(NOTE_C5, 8);
  play(NOTE_H4, 8);
  play(NOTE_A4, 8);
  play(NOTE_C5, 8);
  play(NOTE_A4, 8);
  play(NOTE_C5, 8);
  play(NOTE_A4, 8);
  play(NOTE_E4, 8);
  play(NOTE_D4_S, 8);
  
  play(NOTE_D4, 8);
  play(NOTE_D4, 8);
  play(NOTE_F4, 8);
  play(NOTE_G4_S, 8);
  play(NOTE_H4, 8);
  play(NOTE_D4, 8);
  play(NOTE_F4, 8);
  play(NOTE_G4_S, 8);
  play(NOTE_A4, 8);
  play(NOTE_C4, 8);
  play(NOTE_C4, 8);
  play(NOTE_G4, 8);
  play(NOTE_F4, 8);
  play(NOTE_E4, 8);
  play(NOTE_G4, 8);
  play(NOTE_F4, 8);
  play(NOTE_F4, 8);
  play(NOTE_E4, 8);
  play(NOTE_E4, 8);
  play(NOTE_G4_S, 8);

  play(NOTE_A4, 8);


}

void monkey_island(){
  tempo = 120;
  play(NOTE_H4,4);
  play(NOTE_H4,40);
  play(NOTE_D5,40);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,4);
  play(NOTE_H4,-1);
  play(NOTE_A4,4);
  play(NOTE_A4,40);
  play(NOTE_G4,40);
  play(NOTE_F4_S,40);
  play(NOTE_A4,40);
  play(NOTE_G4,4);
  play(NOTE_G4,4);
  play(NOTE_F4_S,-2);
  play(NOTE_H4,4);
  play(NOTE_H4,-4);
  play(NOTE_D5,8);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,4);
  play(NOTE_H4,-2);
  play(0,8);
  play(NOTE_C5_S,8);
  play(NOTE_D5,4);
  play(NOTE_D5,4);
  play(NOTE_E5,2);

  play(NOTE_C5_S,-4);
  play(NOTE_D5,8);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,40);
  play(NOTE_C5_S,40);
  play(NOTE_D5,4);
  play(NOTE_D5,4);
  play(NOTE_C5_S,2);
  play(NOTE_H4,-4);
  play(NOTE_D5,8);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,40);
  play(NOTE_C5_S,40);
  play(NOTE_D5,4);
  play(NOTE_D5,4);
  play(NOTE_C5_S,2);
  play(NOTE_H4,-4);
  play(NOTE_D5,8);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,40);
  play(NOTE_C5_S,40);
  play(NOTE_D5,4);
  play(NOTE_D5,4);
  play(NOTE_C5_S,2);
  play(NOTE_H4,-4);
  play(NOTE_D5,8);
  play(NOTE_C5_S,40);
  play(NOTE_H4,40);
  play(NOTE_A4,4);
  play(NOTE_H4,4);
  play(NOTE_H4,4);
  play(NOTE_H4,2);
  play(NOTE_H4,-2);
  play(NOTE_H4,8);
  play(NOTE_A4,40);
  play(NOTE_G4,40);
  play(NOTE_F4_S,40);
  play(NOTE_A4,40);

  play(NOTE_G4,4);
  play(NOTE_G4,4);
  play(NOTE_F4_S,2);
  play(NOTE_F4_S,1);
  play(0,1);
  play(NOTE_F4_S,4);
  play(NOTE_A4,4);
  play(NOTE_H4,4);
  play(NOTE_D5,4);
  play(NOTE_D5,2);
  play(NOTE_G5,40);
  play(NOTE_F5_S,40);
  play(NOTE_E5,40);
  play(NOTE_G5,40);
  play(NOTE_F5_S,4);
  play(NOTE_D5,4);
  play(NOTE_D5,2);
  play(0,8);
  play(NOTE_D5,-4);
  play(NOTE_E5,4);
  play(NOTE_A4,4);

  play(NOTE_A4,-4);
  play(NOTE_E5,8);
  play(NOTE_F5_S,40);
  play(NOTE_E5,40);
  play(NOTE_D5,40);
  play(NOTE_E5,40);
  play(NOTE_F5_S,4);
  play(NOTE_D5,4);
  play(NOTE_D5,4);
  play(NOTE_H4,4);
  play(NOTE_H4,2);
  play(NOTE_C5_S,4);
  play(NOTE_D5,4);
  play(NOTE_E5,1);
  play(NOTE_E5,2);
  play(NOTE_E5,2);
  play(NOTE_H4,1);
  play(NOTE_H4,2);

};


void imperial_march() {
  tempo = 120;
	play(NOTE_A4, 6);
	play(NOTE_A4, 6);
	play(NOTE_A4, 16);
  play(NOTE_A4, 16);
	play(NOTE_A4, 16);
	play(NOTE_A4, 16);
  play(NOTE_F4, 8);
  play(0, 8);
	play(NOTE_A4, 6);
	play(NOTE_A4, 6);
	play(NOTE_A4, 16);
  play(NOTE_A4, 16);
	play(NOTE_A4, 16);
	play(NOTE_A4, 16);
  play(NOTE_F4, 8);
  play(0, 8);
  play(NOTE_A4, 4);
  play(NOTE_A4, 4);
  play(NOTE_A4, 4);
  play(NOTE_F4, 12);
  play(NOTE_C5, 16);

  play(NOTE_A4, 4);
  play(NOTE_F4, 12);
  play(NOTE_C5, 16);
  play(NOTE_A4, 2);
  play(NOTE_E5, 4);
  play(NOTE_E5, 4);
  play(NOTE_E5, 4);
  play(NOTE_F5, 12);
  play(NOTE_C5, 16);
  play(NOTE_A4, 4);
  play(NOTE_F4, 12);
  play(NOTE_C5, 16);
  play(NOTE_A4, 2);

  play(NOTE_A5, 4);
  play(NOTE_A4, 12);
  play(NOTE_A4, 16);
  play(NOTE_A5, 4);
  play(NOTE_G5_S, 12);
  play(NOTE_G5, 16);
  play(NOTE_D5_S, 16);
  play(NOTE_D5, 16);
  play(NOTE_D5_S, 8);
  play(0, 8);
  play(NOTE_A4, 8);
  play(NOTE_D5_S, 4);
  play(NOTE_D5, 12);
  play(NOTE_C5_S, 16);

  play(NOTE_C5, 16);
  play(NOTE_H4, 16);
  play(NOTE_C5, 16);
  play(0, 8);
  play(NOTE_F4, 8);
  play(NOTE_G4_S, 4);
  play(NOTE_F4, 12);
  play(NOTE_A4, 24);
  play(NOTE_C5, 4);
  play(NOTE_A4, 12);
  play(NOTE_C5, 16);
  play(NOTE_E5, 2);

  play(NOTE_A5, 4);
  play(NOTE_A4, 12);
  play(NOTE_A4, 16);
  play(NOTE_A5, 4);
  play(NOTE_G5_S, 12);
  play(NOTE_G5, 16);
  play(NOTE_D5_S, 16);
  play(NOTE_D5, 16);
  play(NOTE_D5_S, 8);
  play(0, 8);
  play(NOTE_A4, 8);
  play(NOTE_D5_S, 4);
  play(NOTE_D5, 12);
  play(NOTE_C5_S, 16);

  play(NOTE_C5, 16);
  play(NOTE_H4, 16);
  play(NOTE_C5, 16);
  play(0, 8);
  play(NOTE_F4, 8);
  play(NOTE_G4_S, 4);
  play(NOTE_F4, 12);
  play(NOTE_A4, 24);
  play(NOTE_C5, 4);
  play(NOTE_A4, 12);
  play(NOTE_C5, 16);
  play(NOTE_E5, 2);

};





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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  imperial_march();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
