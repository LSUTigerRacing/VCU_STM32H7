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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

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
/* Definitions for ReadCAN0 */
osThreadId_t ReadCAN0Handle;
uint32_t ReadCAN0BuffBuffer[ 1024 ];
osStaticThreadDef_t ReadCAN0BuffControlBlock;
const osThreadAttr_t ReadCAN0_attributes = {
  .name = "ReadCAN0",
  .cb_mem = &ReadCAN0BuffControlBlock,
  .cb_size = sizeof(ReadCAN0BuffControlBlock),
  .stack_mem = &ReadCAN0BuffBuffer[0],
  .stack_size = sizeof(ReadCAN0BuffBuffer),
  .priority = (osPriority_t) osPriorityAboveNormal5,
};
/* Definitions for ReadCAN1 */
osThreadId_t ReadCAN1Handle;
uint32_t ReadCAN1Buffer[ 1024 ];
osStaticThreadDef_t ReadCAN1ControlBlock;
const osThreadAttr_t ReadCAN1_attributes = {
  .name = "ReadCAN1",
  .cb_mem = &ReadCAN1ControlBlock,
  .cb_size = sizeof(ReadCAN1ControlBlock),
  .stack_mem = &ReadCAN1Buffer[0],
  .stack_size = sizeof(ReadCAN1Buffer),
  .priority = (osPriority_t) osPriorityBelowNormal2,
};
/* Definitions for CAN1rx */
osMessageQueueId_t CAN1rxHandle;
uint8_t CAN1rxBuffer[ 1024 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN1rxControlBlock;
const osMessageQueueAttr_t CAN1rx_attributes = {
  .name = "CAN1rx",
  .cb_mem = &CAN1rxControlBlock,
  .cb_size = sizeof(CAN1rxControlBlock),
  .mq_mem = &CAN1rxBuffer,
  .mq_size = sizeof(CAN1rxBuffer)
};
/* Definitions for CAN1tx */
osMessageQueueId_t CAN1txHandle;
uint8_t CAN1txBuffer[ 128 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN1txControlBlock;
const osMessageQueueAttr_t CAN1tx_attributes = {
  .name = "CAN1tx",
  .cb_mem = &CAN1txControlBlock,
  .cb_size = sizeof(CAN1txControlBlock),
  .mq_mem = &CAN1txBuffer,
  .mq_size = sizeof(CAN1txBuffer)
};
/* Definitions for CAN0rx */
osMessageQueueId_t CAN0rxHandle;
uint8_t CAN0rxBuffer[ 1024 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN0rxControlBlock;
const osMessageQueueAttr_t CAN0rx_attributes = {
  .name = "CAN0rx",
  .cb_mem = &CAN0rxControlBlock,
  .cb_size = sizeof(CAN0rxControlBlock),
  .mq_mem = &CAN0rxBuffer,
  .mq_size = sizeof(CAN0rxBuffer)
};
/* Definitions for CAN0tx */
osMessageQueueId_t CAN0txHandle;
uint8_t CAN0txBuffer[ 128 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN0txControlBlock;
const osMessageQueueAttr_t CAN0tx_attributes = {
  .name = "CAN0tx",
  .cb_mem = &CAN0txControlBlock,
  .cb_size = sizeof(CAN0txControlBlock),
  .mq_mem = &CAN0txBuffer,
  .mq_size = sizeof(CAN0txBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartADCTask(void *argument);
void StartCAN0Read(void *argument);
void StartCAN1Read(void *argument);

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

  /* Create the queue(s) */
  /* creation of CAN1rx */
  CAN1rxHandle = osMessageQueueNew (1024, sizeof(uint8_t), &CAN1rx_attributes);

  /* creation of CAN1tx */
  CAN1txHandle = osMessageQueueNew (128, sizeof(uint8_t), &CAN1tx_attributes);

  /* creation of CAN0rx */
  CAN0rxHandle = osMessageQueueNew (1024, sizeof(uint8_t), &CAN0rx_attributes);

  /* creation of CAN0tx */
  CAN0txHandle = osMessageQueueNew (128, sizeof(uint8_t), &CAN0tx_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ADCTask */
  ADCTaskHandle = osThreadNew(StartADCTask, NULL, &ADCTask_attributes);

  /* creation of ReadCAN0 */
  ReadCAN0Handle = osThreadNew(StartCAN0Read, NULL, &ReadCAN0_attributes);

  /* creation of ReadCAN1 */
  ReadCAN1Handle = osThreadNew(StartCAN1Read, NULL, &ReadCAN1_attributes);

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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartADCTask */
}

/* USER CODE BEGIN Header_StartCAN0Read */
/**
* @brief Function implementing the ReadCAN0 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCAN0Read */
void StartCAN0Read(void *argument)
{
  /* USER CODE BEGIN StartCAN0Read */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCAN0Read */
}

/* USER CODE BEGIN Header_StartCAN1Read */
/**
* @brief Function implementing the ReadCAN1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCAN1Read */
void StartCAN1Read(void *argument)
{
  /* USER CODE BEGIN StartCAN1Read */
  /* Infinite loop */
  for(;;)
  {

    osDelay(1);
  }
  /* USER CODE END StartCAN1Read */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

