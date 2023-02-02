/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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

#define RGB_NUM 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int clock = 32000000;
int frequency;
int ARR;
int CCR;
int note;
int rythm;
int count = 0;
typedef enum {false, true} bool;
bool state = true;
int status_buffer = 0;
int tempo;
int wholenote;
int divide;
int duration;
// typedef struct color{
//   int red;
//   int green;
//   int blue;
// };
// typedef struct color rgb;
int red [RGB_NUM] = {255,0,0};
int orange [RGB_NUM] = {255,127,0};
int yellow [RGB_NUM] = {255,255,0};
int chartreuse [RGB_NUM] = {127,255,0};
int green [RGB_NUM] = {0,255,0};
int spring_green [RGB_NUM] = {0,255,127};
int cyan [RGB_NUM] = {0,255,255};
int azure [RGB_NUM] = {0,127,255};
int blue [RGB_NUM] = {0,0,255};
int violet [RGB_NUM] = {127,0,255};
int magenta [RGB_NUM] = {255,0,255};
int rose [RGB_NUM] = {255,0,127};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM15_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_7 && state == true){
	HAL_TIM_Base_Start_IT(&htim15);
	state = false;

  }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
	//if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET){  // should probably be used
		if(count < 3){
		      count++;
		      }
		      else{
		        count = 0;
		      }
		   // }
		state = true;
		HAL_TIM_Base_Stop_IT(&htim15);

}
int* get_note(int note){
  
	if(note == 131 || note == 261 || note == 523){
		return rose;
	}else if(note == 138 || note == 277 || note == 554){
		return magenta;
	}else if(note == 147 || note == 293 || note == 587){
		return violet;
	}else if(note == 155 || note == 311 || note == 622){
		return blue;
	}else if(note == 165 || note == 329 || note == 659){
		return azure;
	}else if(note == 174 || note == 349 || note == 698){
		return cyan;
	}else if(note == 185 || note == 370 || note == 740){
		return spring_green;
	}else if(note == 196 || note == 392 || note == 784){
		return green;
	}else if(note == 207 || note == 415 || note == 830){
		return chartreuse;
	}else if(note == 220 || note == 440 || note == 880){
		return yellow;
	}else if(note == 233 || note == 466 || note == 932){
		return orange;
	}else{
		return red;
	}
}

int set_color(int note)
{
  int* color = get_note(note);
  int red = *(color + 0);
  int green = *(color + 1);
  int blue = *(color + 2);
  htim1.Instance->CCR1 = red * 257;
  htim1.Instance->CCR2 = green * 257;
  htim1.Instance->CCR3 = blue * 257;

}
int reset_color()
{
  int red;
  int green;
  int blue;
  htim1.Instance->CCR1 = red * 0;
  htim1.Instance->CCR2 = green * 0;
  htim1.Instance->CCR3 = blue * 0;

}

void frequency_change(int note) {
		ARR = clock/note;
		CCR = ARR/2;
		__HAL_TIM_SET_COUNTER(&htim2, 0);
	  __HAL_TIM_SET_AUTORELOAD(&htim2, ARR);
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, CCR);
};


void play_note(int rythm, int note) {
  if(status_buffer == count){
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
  set_color(note);
  HAL_Delay(duration*0.9);
  reset_color();
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
  HAL_Delay(duration);
  }
};

void play(int note, int rythm){
	frequency_change(note);
	play_note(rythm, note);
};

void godfather(){
  status_buffer = 2;
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
  status_buffer = 3;
  tempo = 140;
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
  status_buffer = 0;
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

void tetris(){
  status_buffer = 1;
  tempo = 144;
  play(NOTE_E5, 4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_C5,8);
  play(NOTE_H4,8);
  play(NOTE_A4, 4);
  play(NOTE_A4,8);
  play(NOTE_C5,8);
  play(NOTE_E5,4);
  play(NOTE_D5,8);
  play(NOTE_C5,8);
  play(NOTE_H4, -4);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_E5,4);
  play(NOTE_C5, 4);
  play(NOTE_A4,4);
  play(NOTE_A4,8);
  play(NOTE_A4,4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);

  play(NOTE_D5, -4);
  play(NOTE_F5,8);
  play(NOTE_A5,4);
  play(NOTE_G5,8);
  play(NOTE_F5,8);
  play(NOTE_E5, -4);
  play(NOTE_C5,8);
  play(NOTE_E5,4);
  play(NOTE_D5,8);
  play(NOTE_C5,8);
  play(NOTE_H4, 4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_E5,4);
  play(NOTE_C5, 4);
  play(NOTE_A4,4);
  play(NOTE_A4,4);
  play(0, 4);

  play(NOTE_E5, 4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_C5,8);
  play(NOTE_H4,8);
  play(NOTE_A4, 4);
  play(NOTE_A4,8);
  play(NOTE_C5,8);
  play(NOTE_E5,4);
  play(NOTE_D5,8);
  play(NOTE_C5,8);
  play(NOTE_H4, -4);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_E5,4);
  play(NOTE_C5, 4);
  play(NOTE_A4,4);
  play(NOTE_A4,8);
  play(NOTE_A4,4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);

  play(NOTE_D5, -4);
  play(NOTE_F5,8);
  play(NOTE_A5,4);
  play(NOTE_G5,8);
  play(NOTE_F5,8);
  play(NOTE_E5, -4);
  play(NOTE_C5,8);
  play(NOTE_E5,4);
  play(NOTE_D5,8);
  play(NOTE_C5,8);
  play(NOTE_H4, 4);
  play(NOTE_H4,8);
  play(NOTE_C5,8);
  play(NOTE_D5,4);
  play(NOTE_E5,4);
  play(NOTE_C5, 4);
  play(NOTE_A4,4);
  play(NOTE_A4,4);
  play(0, 4);

  play(NOTE_E5,2);
  play(NOTE_C5,2);
  play(NOTE_D5,2);
  play(NOTE_H4,2);
  play(NOTE_C5,2);
  play(NOTE_A4,2);
  play(NOTE_G4_S,2);
  play(NOTE_H4,4);
  play(0,8);
  play(NOTE_E5,2);
  play(NOTE_C5,2);
  play(NOTE_D5,2);
  play(NOTE_H4,2);
  play(NOTE_C5,4);
  play(NOTE_E5,4);
  play(NOTE_A5,2);
  play(NOTE_G5_S,2);

}

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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (count == 0) {
      imperial_march();
    };
    if (count == 1) {
      tetris();

    };
    if (count == 2) {
      godfather();
    };
    if (count == 3) {
    	monkey_island();
    };

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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 32000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 50;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 65535;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  htim2.Init.Period = 4294967295;
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
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 32000;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 100;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

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

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
