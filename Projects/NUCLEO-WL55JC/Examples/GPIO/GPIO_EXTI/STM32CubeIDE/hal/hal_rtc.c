/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "hal_rtc.h"
#include "stm32wlxx_ll_rtc.h"
#include "utilities.h"

/* USER CODE BEGIN 0 */
/*!
 * \brief Days, Hours, Minutes and seconds
 */
#define DAYS_IN_LEAP_YEAR                           ( ( uint32_t )  366U )
#define DAYS_IN_YEAR                                ( ( uint32_t )  365U )
#define SECONDS_IN_1DAY                             ( ( uint32_t )86400U )
#define SECONDS_IN_1HOUR                            ( ( uint32_t ) 3600U )
#define SECONDS_IN_1MINUTE                          ( ( uint32_t )   60U )
#define MINUTES_IN_1HOUR                            ( ( uint32_t )   60U )
#define HOURS_IN_1DAY                               ( ( uint32_t )   24U )

/*!
 * \brief Correction factors
 */
#define  DAYS_IN_MONTH_CORRECTION_NORM              ( ( uint32_t )0x99AAA0 )
#define  DAYS_IN_MONTH_CORRECTION_LEAP              ( ( uint32_t )0x445550 )

#define CFG_RTCCLK_DIVIDER_CONF 0

#if (CFG_RTCCLK_DIVIDER_CONF == 0)
/**
 * Custom configuration
 * It does not support 1Hz calendar
 * It divides the RTC CLK by 16
 */
#define CFG_RTCCLK_DIV  (16)
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#define CFG_RTC_ASYNCH_PRESCALER (CFG_RTCCLK_DIV - 1)
#define CFG_RTC_SYNCH_PRESCALER (0x7FFF)

#else

#if (CFG_RTCCLK_DIVIDER_CONF == 2)
/**
 * It divides the RTC CLK by 2
 */
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 4)
/**
 * It divides the RTC CLK by 4
 */
#define CFG_RTC_WUCKSEL_DIVIDER (2)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 8)
/**
 * It divides the RTC CLK by 8
 */
#define CFG_RTC_WUCKSEL_DIVIDER (1)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 16)
/**
 * It divides the RTC CLK by 16
 */
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#endif

#define CFG_RTCCLK_DIV              CFG_RTCCLK_DIVIDER_CONF
#define CFG_RTC_ASYNCH_PRESCALER    (CFG_RTCCLK_DIV - 1)
#define CFG_RTC_SYNCH_PRESCALER     (DIVR( LSE_VALUE, \
                                           (CFG_RTC_ASYNCH_PRESCALER + 1) ) - \
                                     1 )

#endif

/** tick timer value in us */
#define CFG_TS_TICK_VAL           DIVR( (CFG_RTCCLK_DIV * 1000000), LSE_VALUE )
#define SECONDS_TO_TICK_VAL(x)				(x*1000*1000/CFG_TS_TICK_VAL)

#define DIVC( X, \
              N )                                ( ( ( X ) + ( N ) -1 ) / \
                                                   ( N ) )

/*!
 * RTC timer context
 */
typedef struct
{
  uint32_t Time;                  // Reference time
  RTC_TimeTypeDef CalendarTime;   // Reference time in calendar format
  RTC_DateTypeDef CalendarDate;   // Reference date in calendar format
}RtcTimerContext_t;

/*!
 * \brief Indicates if the RTC is already Initialized or not
 */
static bool RtcInitialized = false;

/*!
 * Keep the value of the RTC timer when the RTC alarm is set
 * Set with the \ref RtcSetTimerContext function
 * Value is kept as a Reference to calculate alarm
 */
static RtcTimerContext_t RtcTimerContext;

/*!
 * \brief Get the current time from calendar in ticks
 *
 * \param [IN] date           Pointer to RTC_DateStruct
 * \param [IN] time           Pointer to RTC_TimeStruct
 * \retval calendarValue Time in ticks
 */
static uint64_t RtcGetCalendarValue( RTC_DateTypeDef* date, RTC_TimeTypeDef* time );

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void hal_rtc_init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */
  if( RtcInitialized == false )
  {

  /* USER CODE END RTC_Init 1 */
	  /** Initialize RTC Only */
	  hrtc.Instance = RTC;
	  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	  hrtc.Init.AsynchPrediv = RTC_PREDIV_A;
	  hrtc.Init.SynchPrediv = RTC_PREDIV_S;
	  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
	  hrtc.Init.BinMode = RTC_BINARY_ONLY;

    (void) HAL_RTC_Init( &hrtc );

	  /* USER CODE BEGIN Check_RTC_BKUP */
	  /* Disable RTC registers write protection */
	  LL_RTC_DisableWriteProtection(RTC);

	  LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

	  /* Enable RTC registers write protection */
	  LL_RTC_EnableWriteProtection(RTC);

	  // Enable Direct Read of the calendar registers (not through Shadow registers)
	  (void) HAL_RTCEx_EnableBypassShadow( &hrtc );

	  /* USER CODE END Check_RTC_BKUP */

	  /*
	   * Initialize RTC and set the Time and Date
	  */

	  (void) HAL_RTC_DeactivateAlarm( &hrtc, RTC_ALARM_A );
	  (void) HAL_RTCEx_SetSSRU_IT(&hrtc);

	  /** Enable the Alarm A */
	  static const RTC_AlarmTypeDef sAlarm = {
			.BinaryAutoClr = RTC_ALARMSUBSECONDBIN_AUTOCLR_NO,
			.AlarmTime.SubSeconds = 0x0,
			.AlarmMask = RTC_ALARMMASK_NONE,
			.AlarmSubSecondMask = RTC_ALARMSUBSECONDBINMASK_NONE,
			.Alarm = RTC_ALARM_A,
	  };

	  (void) HAL_RTC_SetAlarm_IT(&hrtc, (RTC_AlarmTypeDef *)&sAlarm, 0);
	  (void) RtcSetTimerContext();

	  RtcInitialized = true;
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
  return;
}

void
hal_rtc_deinit(void) {
	HAL_RTC_DeInit(&hrtc);
	__HAL_RCC_RTC_DISABLE();
	return;
}

/*!
 * \brief converts time in ticks to time in ms
 *
 * \param[IN] time in timer ticks
 * \retval returns time in milliseconds
 */
uint32_t RtcTick2Ms( uint32_t tick )
{
  uint32_t seconds = tick >> RTC_N_PREDIV_S;

  tick = tick & RTC_PREDIV_S;
  return ( ( seconds * 1000 ) + ( ( tick * 1000 ) >> RTC_N_PREDIV_S ) );
}

/*!
 * \brief Sets the RTC timer reference, sets also the RTC_DateStruct and RTC_TimeStruct
 *
 * \param none
 * \retval timerValue In ticks
 */
uint32_t RtcSetTimerContext( void )
{
  RtcTimerContext.Time = ( uint32_t )RtcGetCalendarValue(
    &RtcTimerContext.CalendarDate, &RtcTimerContext.CalendarTime );
  return ( uint32_t )RtcTimerContext.Time;
}

uint32_t RtcGetTimerElapsedTime( void )
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  uint32_t calendarValue = ( uint32_t )RtcGetCalendarValue( &date, &time );

  return( ( uint32_t )( calendarValue - RtcTimerContext.Time ) );
}

/*!
 * \brief Gets the RTC timer reference
 *
 * \param none
 * \retval timerValue In ticks
 */
uint32_t RtcGetTimerContext( void )
{
  return RtcTimerContext.Time;
}

/*!
 * \brief Sets the alarm
 *
 * \note The alarm is set at now (read in this function) + timeout
 *
 * \param timeout Duration of the Timer ticks
 */
void RtcSetAlarm( uint32_t timeout )
{

}

void RtcStopAlarm( void )
{

}

void RtcStartAlarm( uint32_t timeout )
{

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
  /** Initializes the peripherals clocks
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(TAMP_STAMP_LSECSS_SSRU_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TAMP_STAMP_LSECSS_SSRU_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_RTCAPB_CLK_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(TAMP_STAMP_LSECSS_SSRU_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

uint64_t RtcGetCalendarValue( RTC_DateTypeDef* date, RTC_TimeTypeDef* time )
{
  uint64_t calendarValue = 0;
  uint32_t firstRead;
  uint32_t correction;
  uint32_t seconds;

  // Make sure it is correct due to asynchronus nature of RTC
  do
  {
    firstRead = RTC->SSR;
    HAL_RTC_GetDate( &hrtc, date, RTC_FORMAT_BIN );
    HAL_RTC_GetTime( &hrtc, time, RTC_FORMAT_BIN );
  }while ( firstRead != RTC->SSR );

  // Calculte amount of elapsed days since 01/01/2000
  seconds = DIVC( ( DAYS_IN_YEAR * 3 + DAYS_IN_LEAP_YEAR ) * date->Year, 4 );

  correction =
    ( ( date->Year % 4 ) ==
      0 ) ? DAYS_IN_MONTH_CORRECTION_LEAP : DAYS_IN_MONTH_CORRECTION_NORM;

  seconds +=
    ( DIVC( ( date->Month - 1 ) * ( 30 + 31 ),
            2 ) -
      ( ( ( correction >> ( ( date->Month - 1 ) * 2 ) ) & 0x03 ) ) );

  seconds += ( date->Date - 1 );

  // Convert from days to seconds
  seconds *= SECONDS_IN_1DAY;

  seconds += ( ( uint32_t )time->Seconds +
               ( ( uint32_t )time->Minutes * SECONDS_IN_1MINUTE ) +
               ( ( uint32_t )time->Hours * SECONDS_IN_1HOUR ) );

  calendarValue = ( ( ( uint64_t )seconds ) << RTC_N_PREDIV_S ) +
                  ( RTC_PREDIV_S - time->SubSeconds );

  return( calendarValue );
}

uint32_t RtcGetCalendarTime( uint16_t *milliseconds )
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  uint32_t ticks;
  uint32_t seconds = 0;

  if( RtcInitialized )
  {

    uint64_t calendarValue = RtcGetCalendarValue( &date, &time );
    seconds = ( uint32_t )( calendarValue >> RTC_N_PREDIV_S );
    ticks =  ( uint32_t )calendarValue & RTC_PREDIV_S;
    *milliseconds = RtcTick2Ms( ticks );

  }
  return seconds;
}

void RtcBkupRegWrite( uint32_t backup_reg, uint32_t data)
{
  HAL_RTCEx_BKUPWrite( &hrtc, backup_reg, data );
}

void RtcBkupRegRead( uint32_t backup_reg, uint32_t* data)
{
  *data = HAL_RTCEx_BKUPRead( &hrtc, backup_reg );
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
