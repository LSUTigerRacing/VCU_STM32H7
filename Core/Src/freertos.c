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
#define CMSIS
#include "fdcan.h"
#include "adc.h"
#include "tim.h"

extern void Prepare_Message(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t dlc);
extern void Decode_Message(CAN_Msg_Raw msg, DBC_Translation dbc);

extern DBC_Translation Assign_Signal(CAN_Msg_Raw *msg);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
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
/* Definitions for DecodeCAN2 */
osThreadId_t DecodeCAN2Handle;
uint32_t DecodeCAN2Buffer[ 1024 ];
osStaticThreadDef_t DecodeCAN2ControlBlock;
const osThreadAttr_t DecodeCAN2_attributes = {
  .name = "DecodeCAN2",
  .cb_mem = &DecodeCAN2ControlBlock,
  .cb_size = sizeof(DecodeCAN2ControlBlock),
  .stack_mem = &DecodeCAN2Buffer[0],
  .stack_size = sizeof(DecodeCAN2Buffer),
  .priority = (osPriority_t) osPriorityBelowNormal2,
};
/* Definitions for DecodeCAN1 */
osThreadId_t DecodeCAN1Handle;
uint32_t DecodeCAN1Buffer[ 1024 ];
osStaticThreadDef_t DecodeCAN1ControlBlock;
const osThreadAttr_t DecodeCAN1_attributes = {
  .name = "DecodeCAN1",
  .cb_mem = &DecodeCAN1ControlBlock,
  .cb_size = sizeof(DecodeCAN1ControlBlock),
  .stack_mem = &DecodeCAN1Buffer[0],
  .stack_size = sizeof(DecodeCAN1Buffer),
  .priority = (osPriority_t) osPriorityAboveNormal5,
};
/* Definitions for CreateMsgCAN1 */
osThreadId_t CreateMsgCAN1Handle;
uint32_t CreateMsgCAN1Buffer[ 1024 ];
osStaticThreadDef_t CreateMsgCAN1ControlBlock;
const osThreadAttr_t CreateMsgCAN1_attributes = {
  .name = "CreateMsgCAN1",
  .cb_mem = &CreateMsgCAN1ControlBlock,
  .cb_size = sizeof(CreateMsgCAN1ControlBlock),
  .stack_mem = &CreateMsgCAN1Buffer[0],
  .stack_size = sizeof(CreateMsgCAN1Buffer),
  .priority = (osPriority_t) osPriorityAboveNormal4,
};
/* Definitions for CreateMsgCAN2 */
osThreadId_t CreateMsgCAN2Handle;
uint32_t CreateMsgCAN2Buffer[ 1024 ];
osStaticThreadDef_t CreateMsgCAN2ControlBlock;
const osThreadAttr_t CreateMsgCAN2_attributes = {
  .name = "CreateMsgCAN2",
  .cb_mem = &CreateMsgCAN2ControlBlock,
  .cb_size = sizeof(CreateMsgCAN2ControlBlock),
  .stack_mem = &CreateMsgCAN2Buffer[0],
  .stack_size = sizeof(CreateMsgCAN2Buffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CAN1rxQ */
osMessageQueueId_t CAN1rxQHandle;
uint8_t CAN1rxQBuffer[ 1024 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN1rxQControlBlock;
const osMessageQueueAttr_t CAN1rxQ_attributes = {
  .name = "CAN1rxQ",
  .cb_mem = &CAN1rxQControlBlock,
  .cb_size = sizeof(CAN1rxQControlBlock),
  .mq_mem = &CAN1rxQBuffer,
  .mq_size = sizeof(CAN1rxQBuffer)
};
/* Definitions for CAN1txQ */
osMessageQueueId_t CAN1txQHandle;
uint8_t CAN1txQBuffer[ 128 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN1txQControlBlock;
const osMessageQueueAttr_t CAN1txQ_attributes = {
  .name = "CAN1txQ",
  .cb_mem = &CAN1txQControlBlock,
  .cb_size = sizeof(CAN1txQControlBlock),
  .mq_mem = &CAN1txQBuffer,
  .mq_size = sizeof(CAN1txQBuffer)
};
/* Definitions for CAN2rxQ */
osMessageQueueId_t CAN2rxQHandle;
uint8_t CAN2rxQBuffer[ 1024 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN2rxQControlBlock;
const osMessageQueueAttr_t CAN2rxQ_attributes = {
  .name = "CAN2rxQ",
  .cb_mem = &CAN2rxQControlBlock,
  .cb_size = sizeof(CAN2rxQControlBlock),
  .mq_mem = &CAN2rxQBuffer,
  .mq_size = sizeof(CAN2rxQBuffer)
};
/* Definitions for CAN2txQ */
osMessageQueueId_t CAN2txQHandle;
uint8_t CAN2txQBuffer[ 128 * sizeof( uint8_t ) ];
osStaticMessageQDef_t CAN2txQControlBlock;
const osMessageQueueAttr_t CAN2txQ_attributes = {
  .name = "CAN2txQ",
  .cb_mem = &CAN2txQControlBlock,
  .cb_size = sizeof(CAN2txQControlBlock),
  .mq_mem = &CAN2txQBuffer,
  .mq_size = sizeof(CAN2txQBuffer)
};
/* Definitions for CAN1txM */
osMutexId_t CAN1txMHandle;
osStaticMutexDef_t CAN1txMControlBlock;
const osMutexAttr_t CAN1txM_attributes = {
  .name = "CAN1txM",
  .cb_mem = &CAN1txMControlBlock,
  .cb_size = sizeof(CAN1txMControlBlock),
};
/* Definitions for CAN1rxM */
osMutexId_t CAN1rxMHandle;
osStaticMutexDef_t CAN1rxMControlBlock;
const osMutexAttr_t CAN1rxM_attributes = {
  .name = "CAN1rxM",
  .cb_mem = &CAN1rxMControlBlock,
  .cb_size = sizeof(CAN1rxMControlBlock),
};
/* Definitions for CAN2txM */
osMutexId_t CAN2txMHandle;
osStaticMutexDef_t CAN2txMControlBlock;
const osMutexAttr_t CAN2txM_attributes = {
  .name = "CAN2txM",
  .cb_mem = &CAN2txMControlBlock,
  .cb_size = sizeof(CAN2txMControlBlock),
};
/* Definitions for CAN2rxM */
osMutexId_t CAN2rxMHandle;
osStaticMutexDef_t CAN2rxMControlBlock;
const osMutexAttr_t CAN2rxM_attributes = {
  .name = "CAN2rxM",
  .cb_mem = &CAN2rxMControlBlock,
  .cb_size = sizeof(CAN2rxMControlBlock),
};
/* Definitions for CAN1rxS */
osSemaphoreId_t CAN1rxSHandle;
osStaticSemaphoreDef_t CAN1rxSControlBlock;
const osSemaphoreAttr_t CAN1rxS_attributes = {
  .name = "CAN1rxS",
  .cb_mem = &CAN1rxSControlBlock,
  .cb_size = sizeof(CAN1rxSControlBlock),
};
/* Definitions for CAN1txS */
osSemaphoreId_t CAN1txSHandle;
osStaticSemaphoreDef_t CAN1txSControlBlock;
const osSemaphoreAttr_t CAN1txS_attributes = {
  .name = "CAN1txS",
  .cb_mem = &CAN1txSControlBlock,
  .cb_size = sizeof(CAN1txSControlBlock),
};
/* Definitions for CAN2rxS */
osSemaphoreId_t CAN2rxSHandle;
osStaticSemaphoreDef_t CAN2rxSControlBlock;
const osSemaphoreAttr_t CAN2rxS_attributes = {
  .name = "CAN2rxS",
  .cb_mem = &CAN2rxSControlBlock,
  .cb_size = sizeof(CAN2rxSControlBlock),
};
/* Definitions for CAN2txS */
osSemaphoreId_t CAN2txSHandle;
osStaticSemaphoreDef_t CAN2txSControlBlock;
const osSemaphoreAttr_t CAN2txS_attributes = {
  .name = "CAN2txS",
  .cb_mem = &CAN2txSControlBlock,
  .cb_size = sizeof(CAN2txSControlBlock),
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
void StartDecodeCAN2(void *argument);
void StartDecodeCAN1(void *argument);
void StartCreateMsgCAN1(void *argument);
void StartCreateMsgCAN2(void *argument);
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
  /* Create the mutex(es) */
  /* creation of CAN1txM */
  CAN1txMHandle = osMutexNew(&CAN1txM_attributes);

  /* creation of CAN1rxM */
  CAN1rxMHandle = osMutexNew(&CAN1rxM_attributes);

  /* creation of CAN2txM */
  CAN2txMHandle = osMutexNew(&CAN2txM_attributes);

  /* creation of CAN2rxM */
  CAN2rxMHandle = osMutexNew(&CAN2rxM_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of CAN1rxS */
  CAN1rxSHandle = osSemaphoreNew(1, 1, &CAN1rxS_attributes);

  /* creation of CAN1txS */
  CAN1txSHandle = osSemaphoreNew(1, 1, &CAN1txS_attributes);

  /* creation of CAN2rxS */
  CAN2rxSHandle = osSemaphoreNew(1, 1, &CAN2rxS_attributes);

  /* creation of CAN2txS */
  CAN2txSHandle = osSemaphoreNew(1, 1, &CAN2txS_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CAN1rxQ */
  CAN1rxQHandle = osMessageQueueNew (1024, sizeof(uint8_t), &CAN1rxQ_attributes);

  /* creation of CAN1txQ */
  CAN1txQHandle = osMessageQueueNew (128, sizeof(uint8_t), &CAN1txQ_attributes);

  /* creation of CAN2rxQ */
  CAN2rxQHandle = osMessageQueueNew (1024, sizeof(uint8_t), &CAN2rxQ_attributes);

  /* creation of CAN2txQ */
  CAN2txQHandle = osMessageQueueNew (128, sizeof(uint8_t), &CAN2txQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ADCTask */
  ADCTaskHandle = osThreadNew(StartADCTask, NULL, &ADCTask_attributes);

  /* creation of DecodeCAN2 */
  DecodeCAN2Handle = osThreadNew(StartDecodeCAN2, NULL, &DecodeCAN2_attributes);

  /* creation of DecodeCAN1 */
  DecodeCAN1Handle = osThreadNew(StartDecodeCAN1, NULL, &DecodeCAN1_attributes);

  /* creation of CreateMsgCAN1 */
  CreateMsgCAN1Handle = osThreadNew(StartCreateMsgCAN1, NULL, &CreateMsgCAN1_attributes);

  /* creation of CreateMsgCAN2 */
  CreateMsgCAN2Handle = osThreadNew(StartCreateMsgCAN2, NULL, &CreateMsgCAN2_attributes);
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

/* USER CODE BEGIN Header_StartDecodeCAN2 */
/**
* @brief Function implementing the DecodeCAN2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDecodeCAN2 */
void StartDecodeCAN2(void *argument)
{
  /* USER CODE BEGIN StartDecodeCAN2 */
  CAN_Msg_Raw msg_raw;
  DBC_Translation dbc;
  osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
    osSemaphoreAcquire(CAN2rxSHandle, 0);
    status = osMessageQueueGet(CAN2rxQHandle, &msg_raw, NULL, osWaitForever);
    switch(status){
      case osOK:
        dbc = Assign_Signal(&msg_raw);
        Decode_Message(msg_raw, dbc);
        osSemaphoreRelease(CAN2rxSHandle);
      break;

      default:
      Error_Handler();
    }
    osDelay(1);
  }
  /* USER CODE END StartDecodeCAN2 */
}

/* USER CODE BEGIN Header_StartDecodeCAN1 */
/**
* @brief Function implementing the DecodeCAN1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDecodeCAN1 */
void StartDecodeCAN1(void *argument)
{
  /* USER CODE BEGIN StartDecodeCAN1 */
  CAN_Msg_Raw msg_raw;
  DBC_Translation dbc;
  osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
   osSemaphoreAcquire(CAN1rxSHandle, 0);
    status = osMessageQueueGet(CAN1rxQHandle, &msg_raw, NULL, osWaitForever);
    switch(status){
      case osOK:
        dbc = Assign_Signal(&msg_raw);
        Decode_Message(msg_raw, dbc);
        osSemaphoreRelease(CAN1rxSHandle);
      break;

      default:
      Error_Handler();
    } 
    osDelay(1);
  }
  /* USER CODE END StartDecodeCAN1 */
}

/* USER CODE BEGIN Header_StartCreateMsgCAN1 */
/**
* @brief Function implementing the CreateMsgCAN1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCreateMsgCAN1 */
void StartCreateMsgCAN1(void *argument)
{
  /* USER CODE BEGIN StartCreateMsgCAN1 */
  /* Infinite loop */
  for(;;)
  {
    //implement if needed
    osDelay(1);
  }
  /* USER CODE END StartCreateMsgCAN1 */
}

/* USER CODE BEGIN Header_StartCreateMsgCAN2 */
/**
* @brief Function implementing the CreateMsgCAN2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCreateMsgCAN2 */
void StartCreateMsgCAN2(void *argument)
{
  /* USER CODE BEGIN StartCreateMsgCAN2 */
  /* Infinite loop */
  for(;;)
  {
    //implement if needed
    osDelay(1);
  }
  /* USER CODE END StartCreateMsgCAN2 */
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
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  uint16_t pump_on = 900; // 900/1000 = 90% duty cycle
  uint16_t pump_off = 100; // 100/1000 = 10% duty cycle
  /* Infinite loop */
  for(;;)
  {
    test_reading = inlet_temp.avg;

    if (test_reading >= 60000) {
      pump_speed = pump_on; 
    } else if (test_reading <= 10000) {
      pump_speed = pump_off;
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pump_speed);
    osDelay(1);
  }
  /* USER CODE END StartPWMTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

