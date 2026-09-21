/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define D2_RAM __attribute__((section(".D2_RAM")))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern TIM_HandleTypeDef htim1;
extern ADC_HandleTypeDef hadc1;

volatile D2_RAM uint32_t adc12_dma_buf[ADC12_BUFFER_COUNT];
volatile D2_RAM uint32_t adc3_dma_buf[ADC3_BUFFER_COUNT];

volatile uint32_t test_reading;
volatile uint32_t pump_speed;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ADCTask */
osThreadId_t ADCTaskHandle;
const osThreadAttr_t ADCTask_attributes = {
  .name = "ADCTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal3,
};
/* Definitions for PWMTask */
osThreadId_t PWMTaskHandle;
const osThreadAttr_t PWMTask_attributes = {
  .name = "PWMTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal7,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartADCTask(void *argument);
void StartPWMTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ADCTask */
  ADCTaskHandle = osThreadNew(StartADCTask, NULL, &ADCTask_attributes);

  /* creation of PWMTask */
  PWMTaskHandle = osThreadNew(StartPWMTask, NULL, &PWMTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartADCTask */
/**
* @brief Function implementing the ADCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartADCTask */
void StartADCTask(void *argument)
{
  /* USER CODE BEGIN StartADCTask */

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

  HAL_TIM_Base_Start(&htim1);

  /* Infinite loop */
  for(;;)
  {
    HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*) adc12_dma_buf, ADC12_BUFFER_COUNT);

    sus_fl.avg = (sus_fl.data1 + sus_fl.data2) / 2;
    sus_fr.avg = (sus_fr.data1 + sus_fr.data2) / 2;
    sus_bl.avg = (sus_bl.data1 + sus_bl.data2) / 2;
    sus_br.avg = (sus_br.data1 + sus_br.data2) / 2;

    steering_ang.avg = (steering_ang.data1 + steering_ang.data2) / 2;

    throttle_pos.avg = (throttle_pos.data1 + throttle_pos.data2) / 2;

    inlet_temp.avg = (inlet_temp.data1 + inlet_temp.data2) / 2;
    outlet_temp.avg = (outlet_temp.data1 + outlet_temp.data2) / 2;

    f_brake_press.avg = (f_brake_press.data1 + f_brake_press.data2) / 2;
    b_brake_press.avg = (b_brake_press.data1 + b_brake_press.data2) / 2;

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
  /* USER CODE END StartADCTask */
}

/* USER CODE BEGIN Header_StartPWMTask */
/**
* @brief Function implementing the PWMTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPWMTask */
void StartPWMTask(void *argument)
{
  /* USER CODE BEGIN StartPWMTask */
  uint32_t counter_min = 2750;
  uint32_t counter_max = 5500;
  uint16_t adc_max_val = 0xFFFF;

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* Infinite loop */
  for(;;)
  {
    test_reading = f_brake_press.avg;
    pump_speed = counter_min + ((test_reading * (counter_max - counter_min)) / adc_max_val);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pump_speed);
    osDelay(1);
  }
  /* USER CODE END StartPWMTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

