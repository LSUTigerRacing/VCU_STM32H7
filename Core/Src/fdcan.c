/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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

  //CAN0 -> Critical Powertrain functionality CAN1 -> Sensor data (read only)
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "fdcan.h"

/* USER CODE BEGIN 0 */

/* defines ------------------------------------------------------------------*/
#define BYTE1 8
#define BYTE2 16
#define BYTE3 24
#define BYTE4 32
#define BYTE5 40
#define BYTE6 48
#define BYTE7 56
/* End of defines ------------------------------------------------------------------*/

/* Extern Variables ------------------------------------------------------------------*/

/* End of Extern Variables ------------------------------------------------------------------*/

/* Extern Functions ------------------------------------------------------------------*/

/*End of Extern Functions ------------------------------------------------------------------*/

/* Private Variables ------------------------------------------------------------------*/
static uint8_t fdcan0_busy;
static uint8_t fdcan1_busy;
/* End of Private Variables ------------------------------------------------------------------*/

/* Private Functions ------------------------------------------------------------------*/
static void Assign_Signal(CAN_Msg *msg);

/* End of Private Functions ------------------------------------------------------------------*/

/* Global Variables  ------------------------------------------------------------------*/
typedef struct{
  uint8_t id;
  uint8_t dlc;
  uint64_t data;
  CAN_Signal *signal;
} CAN_Msg;

typedef struct{
  uint8_t start_bit;
  uint8_t end_bit;
  uint32_t size;
  float scale;
  int16_t offset;
  float min;
  float max;
}CAN_Signal;
/* End of Global Variables ------------------------------------------------------------------*/

/* Global Functions  ------------------------------------------------------------------*/
CAN_Msg Prepare_Message(uint32_t id, uint8_t *data, uint8_t dlc);
void Decode_Message(CAN_Msg *msg);
/* End of Global Functions ------------------------------------------------------------------*/
/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 16;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 1;
  hfdcan1.Init.NominalTimeSeg2 = 1;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 16;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 32;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 32;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_QUEUE_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
 //Interrupt for queue mode
  HAL_FDCAN_ActivateNotification(&hfdcan1,
    FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_ERROR_WARNING | FDCAN_IT_BUS_OFF | FDCAN_IT_TX_COMPLETE,
    0);

  /* USER CODE END FDCAN1_Init 2 */

}
/* FDCAN2 init function */
void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = ENABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 16;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 1;
  hfdcan2.Init.NominalTimeSeg2 = 1;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 1;
  hfdcan2.Init.DataTimeSeg2 = 1;
  hfdcan2.Init.MessageRAMOffset = 0;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.RxFifo0ElmtsNbr = 32;
  hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxFifo1ElmtsNbr = 32;
  hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxBuffersNbr = 0;
  hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.TxEventsNbr = 0;
  hfdcan2.Init.TxBuffersNbr = 32;
  hfdcan2.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */
  //Interrupts for FIFO mode
  HAL_FDCAN_ActivateNotification(&hfdcan2,
    FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_ERROR_WARNING | FDCAN_IT_BUS_OFF | FDCAN_IT_TX_FIFO_EMPTY,
    0);
  /* USER CODE END FDCAN2_Init 2 */

}
/* FDCAN3 init function */
void MX_FDCAN3_Init(void)
{

  /* USER CODE BEGIN FDCAN3_Init 0 */

  /* USER CODE END FDCAN3_Init 0 */

  /* USER CODE BEGIN FDCAN3_Init 1 */

  /* USER CODE END FDCAN3_Init 1 */
  hfdcan3.Instance = FDCAN3;
  hfdcan3.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan3.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan3.Init.AutoRetransmission = DISABLE;
  hfdcan3.Init.TransmitPause = DISABLE;
  hfdcan3.Init.ProtocolException = DISABLE;
  hfdcan3.Init.NominalPrescaler = 16;
  hfdcan3.Init.NominalSyncJumpWidth = 1;
  hfdcan3.Init.NominalTimeSeg1 = 1;
  hfdcan3.Init.NominalTimeSeg2 = 1;
  hfdcan3.Init.DataPrescaler = 1;
  hfdcan3.Init.DataSyncJumpWidth = 1;
  hfdcan3.Init.DataTimeSeg1 = 1;
  hfdcan3.Init.DataTimeSeg2 = 1;
  hfdcan3.Init.MessageRAMOffset = 0;
  hfdcan3.Init.StdFiltersNbr = 0;
  hfdcan3.Init.ExtFiltersNbr = 0;
  hfdcan3.Init.RxFifo0ElmtsNbr = 0;
  hfdcan3.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan3.Init.RxFifo1ElmtsNbr = 0;
  hfdcan3.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan3.Init.RxBuffersNbr = 0;
  hfdcan3.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan3.Init.TxEventsNbr = 0;
  hfdcan3.Init.TxBuffersNbr = 32;
  hfdcan3.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan3.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan3.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN3_Init 2 */

  /* USER CODE END FDCAN3_Init 2 */

}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED=0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspInit 0 */

  /* USER CODE END FDCAN2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN2 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspInit 1 */

  /* USER CODE END FDCAN2_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
  /* USER CODE BEGIN FDCAN3_MspInit 0 */

  /* USER CODE END FDCAN3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN3 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**FDCAN3 GPIO Configuration
    PD12     ------> FDCAN3_RX
    PD13     ------> FDCAN3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_FDCAN3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN FDCAN3_MspInit 1 */

  /* USER CODE END FDCAN3_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

  /* USER CODE END FDCAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* FDCAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

  /* USER CODE END FDCAN2_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
  /* USER CODE BEGIN FDCAN3_MspDeInit 0 */

  /* USER CODE END FDCAN3_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN3 GPIO Configuration
    PD12     ------> FDCAN3_RX
    PD13     ------> FDCAN3_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12|GPIO_PIN_13);

  /* USER CODE BEGIN FDCAN3_MspDeInit 1 */

  /* USER CODE END FDCAN3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/// @brief Adds data to be sent over CAN0 
/// @param data The data that wants to be sent over
void FDCAN0_Tx(uint8_t *data, uint32_t id){
  HAL_StatusTypeDef status;
  FDCAN_TxHeaderTypeDef header;
  header.Identifier = id;
  header.TxFrameType         = FDCAN_DATA_FRAME;
  header.DataLength          = FDCAN_DLC_BYTES_8;
  header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  header.BitRateSwitch       = FDCAN_BRS_OFF;       // OFF if you're not using CAN FD bit-rate switching
  header.FDFormat            = FDCAN_CLASSIC_CAN;   // or FDCAN_FD_CAN if using FD frames
  header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // unless you're using the Tx event FIFO
  header.MessageMarker       = 0;                   // only matters if using Tx event FIFO

  status = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&header,data); 

  switch(status){
    case HAL_OK:

    break;

    case HAL_BUSY:
    fdcan0_busy++;
    break;

    default:
    Error_Handler();
    break;
  }
}


/// @brief Handles recieve for CAN RxFifo0 buffer
/// @param hfdcan Pointer for CAN peripheral
/// @param RxFifo0ITs Interrupt registerbits
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
  if(RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE){
    HAL_StatusTypeDef status;
    uint8_t* data;
    FDCAN_RxHeaderTypeDef header;
    
    status = HAL_FDCAN_GetRxMessage(&hfdcan,FDCAN_RX_FIFO0,&header, data);

    switch(status){
      case HAL_OK:
      CAN_Msg recieved;
      recieved.id = header.Identifier;
      recieved.dlc = header.DataLength;
      recieved.data |= data[7] << BYTE7 | data[6] << BYTE6 | data[5] << BYTE5 | 
      data[4] << BYTE4 | data[3] << BYTE3 | data[2] << BYTE2 | data[1] << BYTE1 | data[0];
      Assign_Signal(&recieved);
      break;

      case HAL_BUSY:
      fdcan0_busy++;
      break;

      default:
      Error_Handler();
      break;
    }
  }
}

/// @brief Handles recieve for CAN RxFifo1 buffer
/// @param hfdcan Pointer for CAN peripheral
/// @param RxFifo1ITs Interrupt
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs){
  if(RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE){
    HAL_StatusTypeDef status;  
    uint8_t* data;
    FDCAN_RxHeaderTypeDef header;

    status = HAL_FDCAN_GetRxMessage(&hfdcan,FDCAN_RX_FIFO1,&header, data);

    switch(status){
      case HAL_OK:
      Encode_Message(&header,&data);
      break;

      case HAL_BUSY:
      fdcan1_busy++;
      break;

      default:
      Error_Handler();
      break;
    }
  }
}

/// @brief Decodes CAN messages based on .dbc files
/// @param msg utilized to decode message
void Decode_Message(CAN_Msg *msg){
  if(msg->id == 999){
    return; //error case
  }

  
}

/// @brief Adds messages to CAN2 FIFO
/// @param hfdcan pointer to can
void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan){

}

/// @brief Preparing a message that is about to be sent
/// @param id CAN id
/// @param data CAN data
/// @return Prepared CAN message
CAN_Msg Prepare_Message(uint32_t id, uint8_t *data, uint8_t dlc){
  CAN_Msg ret;
  ret.id = id;
  ret.dlc = dlc;
  ret.data = &data[0];

  return ret;
}

/// @brief 
/// @param hfdcan 
/// @param BufferIndexes 
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes){
  
}

/// @brief Should be when message is recieved and 
/// @param msg Pointer to CAN message
void Assign_Signal(CAN_Msg *msg){
  CAN_Signal sig;
  switch(msg->id){
    case 10:
    //implement actual dbc definition
    sig.start_bit = 0;
    sig.end_bit = 7;
    sig.offset = .5;
    sig.scale = .25;
    sig.max = 220;
    sig.min = 10;
    msg->signal = &sig;
    break;

    default:
    msg->id = 999; //dbc message not tracked
  }
}
/* USER CODE END 1 */

