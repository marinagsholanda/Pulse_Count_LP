/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through
  *          the STM32WLxx HAL API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
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

#include <stdbool.h>
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "timer_if.h"
#include "stm32wlxx_hal.h"
#include "stm32_lpm_if.h"
#include "SEGGER_RTT.h"
#define PRINTF(...) (void)SEGGER_RTT_printf(0,__VA_ARGS__)
#define PRINT(ENABLE,...) \
	if (ENABLE) { (void)SEGGER_RTT_printf(0,__VA_ARGS__); }
/* Private includes ----------------------------------------------------------*/

/* Task ID definition */
#define TASK_SAMPLING_BT1     	1 << 0
#define TASK_SAMPLING_BT2     	1 << 1
#define TASK_PRINTPULSECNT		1 << 2
#define ALL_TASKS   	  		0xFFFFFFFFu

void SystemClock_Config(void);
static void EXTI2_IRQHandler_Config(void);
static void EXTI0_IRQHandler_Config(void);
void Config_GPIO_ForLP(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

uint32_t pulse_cnt = 0;
bool is_sampling = false;
static UTIL_TIMER_Object_t
  timer_1ms = { 0 };

void task_sampling_bt1(void)
{
	static uint32_t pin_set_cnt = 0;
	static uint32_t samples_cnt = 0;

	//Verificar se o pino está setado e registrar
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
	{
		pin_set_cnt++;
	}

	//Encrementar o número de amostras
	samples_cnt++;

	//Verificar se existem amostras o suficiente
	if (samples_cnt < 10)
	{
		return;
	}

	samples_cnt = 0;
	UTIL_TIMER_Stop(&timer_1ms);

	//Verificar se existe no mínimo 80% de sucesso
	if (pin_set_cnt >= 8)
	{
		pulse_cnt++;
		is_sampling = true;
	}

	pin_set_cnt = 0;

	//Registrou nova amostra
	if (is_sampling == true)
	{
		is_sampling = false;
		PRINT(1,"New Sample: %u.\n", pulse_cnt);
	}
}

uint32_t magnet_cnt = 0;
static UTIL_TIMER_Object_t
  timer_3ms = { 0 };
void task_sampling_bt2(void)
{
	static uint32_t pin_set_counter = 0;
	static uint32_t samples_counter = 0;

	//Verificar se o pino está setado e registrar
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
	{
		pin_set_counter++;
	}

	//Encrementar o número de amostras
	samples_counter++;

	//Verificar se existem amostras o suficiente
	if (samples_counter < 10)
	{
		return;
	}

	samples_counter = 0;
	UTIL_TIMER_Stop(&timer_3ms);

	//Verificar se existe no mínimo 80% de sucesso
	if (pin_set_counter >= 8)
	{
		magnet_cnt++;
	}

	pin_set_counter = 0;

	if (magnet_cnt == 3)
	{
		PRINT(1,"Total sample count: %u.\n", pulse_cnt);
		magnet_cnt = 0;
	}
}

void task_print_pulse_count(void)
{
  PRINT(1,"Total sample count: %u.\n", pulse_cnt);
}

/* IDLE task function */
void UTIL_SEQ_Idle( void )
{
	UTIL_LPM_EnterLowPower();
}

void timer_20s_callback(void * p_arg)
{
	UTIL_SEQ_SetTask(TASK_PRINTPULSECNT, 0);
	return;
}

void timer_1ms_callback(void * p_arg)
{
	UTIL_SEQ_SetTask(TASK_SAMPLING_BT1, 0);
	return;
}

void timer_3ms_callback(void * p_arg)
{
	UTIL_SEQ_SetTask(TASK_SAMPLING_BT2, 0);
	return;
}

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* STM32WLxx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  extern uint8_t _segger_load_;
  extern uint8_t _segger_addr_;
  extern uint8_t _segger_size_;

  (void) memcpy((void *)&_segger_addr_,
	            (const void *)&_segger_load_,
	            (size_t)&_segger_size_);

  HAL_Init();

  SystemClock_Config();

  SEGGER_RTT_Init();

  HAL_DBGMCU_EnableDBGStopMode();

  BSP_LED_Init(LED3);

  UTIL_SEQ_Init();

  UTIL_TIMER_Init();

  UTIL_LPM_DeInit();

  EXTI2_IRQHandler_Config();
  EXTI0_IRQHandler_Config();

  extern RTC_HandleTypeDef hrtc;
  (void) HAL_RTC_RegisterCallback(&hrtc,
  	                                HAL_RTC_ALARM_A_EVENT_CB_ID,
									HAL_RTC_AlarmAEventCallback);

  //Registrando as tasks
  UTIL_SEQ_RegTask(TASK_SAMPLING_BT1,0,task_sampling_bt1);
  UTIL_SEQ_RegTask(TASK_SAMPLING_BT2,0,task_sampling_bt2);
  UTIL_SEQ_RegTask(TASK_PRINTPULSECNT,0,task_print_pulse_count);

  UTIL_LPM_SetStopMode(1 << 0, UTIL_LPM_ENABLE);
  UTIL_LPM_SetOffMode(1 << 0, UTIL_LPM_DISABLE);

  PRINT(1,"Start code.\n");
  for (int i = 0; i < 10; i++)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(200);
  }

  Config_GPIO_ForLP();

  static UTIL_TIMER_Object_t
  timer_20s = { 0 };

  UTIL_TIMER_Create(&timer_20s,20000,UTIL_TIMER_PERIODIC,&timer_20s_callback,NULL);
  UTIL_TIMER_Create(&timer_1ms,1,UTIL_TIMER_PERIODIC,&timer_1ms_callback,NULL);
  UTIL_TIMER_Create(&timer_3ms,3,UTIL_TIMER_PERIODIC,&timer_3ms_callback,NULL);

  /* USER CODE END 2 */
#if 0
  UTIL_TIMER_Start(&timer_20s);
#endif
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 UTIL_SEQ_Run(ALL_TASKS);
  }
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
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON,
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_PWR_DisableBkUpAccess();
}

static void EXTI2_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;


  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;


  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = BUTTON_SW1_PIN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* Enable and set line 0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

static void EXTI0_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;


  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;


  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = BUTTON_SW2_PIN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* Enable and set line 0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void Config_GPIO_ForLP(void)
{
  //return;
  GPIO_InitTypeDef   GPIO_InitStructure;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  GPIO_InitStructure.Pin = GPIO_PIN_All & ~(GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14);
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = GPIO_PIN_All & ~(GPIO_PIN_3);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //Autoriza a amostra se o botão for setado
  if (GPIO_Pin == BUTTON_SW1_PIN)
  {
	  UTIL_TIMER_Start(&timer_1ms);
  }

  else if (GPIO_Pin == BUTTON_SW2_PIN)
  {
	  UTIL_TIMER_Start(&timer_3ms);
  }
  return;
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

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
