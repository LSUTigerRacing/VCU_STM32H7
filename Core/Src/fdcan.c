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

#define BYTE1 8
#define BYTE2 16
#define BYTE3 24
#define BYTE4 32
#define BYTE5 40
#define BYTE6 48
#define BYTE7 56
#define SIGNED BIT0
#define BIG_ENDIAN BIT1
#define TYPE_INT8 BIT2
#define TYPE_INT16 BIT3
#define TYPE_INT24 BIT4
#define TYPE_INT32 BIT5
#define TYPE_FLOAT BIT6

#define BMS1_MSG_INDEX 0
#define BMS2_MSG_INDEX 1
#define DTI1_MSG_INDEX 2
#define DTI2_MSG_INDEX 3
#define DIT3_MSG_INDEX 4
#define DTI4_MSG_INDEX 5
#define DIT5_MSG_INDEX 6
#define COOL_IN_MSG_INDEX 7
#define COOL_OUT_MSG_INDEX 8
#define COOL_FLOW_MSG_INDEX 9
#define BRAKE_PSI_MSG_INDEX 10
#define WHEEL1_MSG_INDEX 11
#define WHEEL2_MSG_INDEX 12
#define ROTOR1_MSG_INDEX 13
#define ROTOR2_MSG_INDEX 14

extern osThreadId_t DecodeCAN1Handle;
extern osThreadId_t DecodeCAN2Handle;
extern osMessageQueueId_t CAN1rxQHandle;
extern osMessageQueueId_t CAN1txQHandle;
extern osMessageQueueId_t CAN2rxQHandle;
extern osMessageQueueId_t CAN2txQHandle;
extern osMutexId_t CAN1rxMHandle;
extern osMutexId_t CAN1txMHandle;
extern osMutexId_t CAN2rxMHandle;
extern osMutexId_t CAN2txMHandle;
extern osSemaphoreId_t CAN1rxSHandle;
extern osSemaphoreId_t CAN1txSHandle;
extern osSemaphoreId_t CAN2rxSHandle;
extern osSemaphoreId_t CAN2txSHandle;

extern uint64_t extract_bits(uint64_t data, uint8_t start_bit, uint8_t size);

typedef struct{
  uint16_t id;
  CAN_Decoded_Values *decoded;
}CAN_Decoded;

static CAN_Decoded CAN_Storage[50];
static uint8_t fdcan0_busy;
static uint8_t fdcan1_busy;

CAN_Decoded_Values Get_Signal(uint16_t id, uint8_t index);
CAN_Decoded_Values* Get_Message(uint16_t id);
static void Store_Message(CAN_Decoded msg, uint8_t sig_count);
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
  hfdcan1.Init.TxBuffersNbr = 2;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 2;
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
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
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
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
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

/// @brief Preparing a message that is about to be sent
/// @param hfdcan CAN handler pointer
/// @param id CAN id
/// @param data CAN data
/// @param dlc Data length code
void Prepare_Message(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t dlc){
  CAN_Msg_Raw ret;

  ret.id = id;
  ret.dlc = dlc;

  for(uint8_t i = 0; i < 8; i++){
    ret.data[i] = data[i];
  }
  if(hfdcan->Instance == FDCAN1){
    osSemaphoreAcquire(CAN1txSHandle, 0);
    osMessageQueuePut(CAN1txQHandle, &ret, 0, 0);
    osSemaphoreRelease(CAN1txSHandle);
  }
  else if(hfdcan->Instance == FDCAN2){
    osSemaphoreAcquire(CAN2txSHandle, 0);
    osMessageQueuePut(CAN2txQHandle, &ret, 0, 0);
    osSemaphoreRelease(CAN2txSHandle);
  }
}

/// @brief Stores decoded CAN message in CAN_Storage array
/// @param msg Decoded CAN message
static void Store_Message(CAN_Decoded msg, uint8_t sig_count){
  uint8_t can_index;
  switch(msg.id){
    case 0x6B1:
      can_index = BMS1_MSG_INDEX;
      break;

    default:
      return; //untracked message
  }

  for(uint8_t i = 0; i < sig_count; i++){
    CAN_Storage[can_index].decoded[i] = msg.decoded[i];
  }
}

/// @brief Assigns signals to the message and should be called after recieving message 
/// @param msg Pointer to CAN message
DBC_Translation Assign_Signal(CAN_Msg_Raw *msg){
    DBC_Translation dbc;

    dbc.sig_count = 0;
    dbc.signal = NULL;

    switch(msg->id){
      case 10:
      //implement actual dbc definition
      CAN_Signal sig[] = {
        {.start_bit = 0, .bit_length = 7, .min = 0, .max = 88, .offset = 0.5, .scale = 0.75, .type = 0}
      };
      
      dbc.signal = sig;
      dbc.sig_count = sizeof(sig)/sizeof(sig[0]);

      return dbc;
      break;

      default:
      msg->id = 999; //dbc message not tracked
    }

    return dbc;
}

/// @brief Decodes CAN messages based on .dbc files
/// @param msg utilized to decode message
///@param dbc The dbc translation for the message
void Decode_Message(CAN_Msg_Raw msg, DBC_Translation dbc){
  if(msg.id == 999){
    return; //error case
  }

  uint64_t translated_data;
  CAN_Decoded ret;
  CAN_Decoded_Values values[dbc.sig_count];
  
  dbc.id = msg.id;
  translated_data = 0;

  for(uint8_t i = 0; i < 8; i++){
    translated_data |= (msg.data[i]) << (i * 8);
  }
  
  for(uint8_t i =0; i < dbc.sig_count;i++ ){
    CAN_Signal *sig = &dbc.signal[i];
    uint8_t size;

    size = dbc.signal->bit_length;
    //Signed + Big Endian
    if(sig->type & SIGNED && sig->type & BIG_ENDIAN){
      //worry about if there are big endian sigs
    } 
    //Signed + Little Endian
    else if(sig->type & SIGNED && !(sig->type & BIG_ENDIAN)){
      uint64_t raw;
      int64_t signed_raw;

      raw = extract_bits(translated_data, sig->start_bit, size);
      signed_raw = (int64_t) raw;
      if(raw & (1ULL << (size - 1))){
        signed_raw -= (1ULL << size);
      }

      if(sig->type & TYPE_FLOAT){
        float decode;

        decode = (float)signed_raw;
        decode *= sig->scale;
        decode += sig->offset;

        values[i].flt = decode;
      }
      else{
        signed_raw *= sig->scale;
        signed_raw += sig->offset;

        if(sig->type & TYPE_INT8){
          values[i].int8 = (int8_t) signed_raw;
          
        }
        else if(sig->type & TYPE_INT16){
          values[i].int16 = (int16_t) signed_raw;
        }
        else if(sig->type & TYPE_INT24){
          values[i].int24 = (int32_t) signed_raw;
        }
        else if(sig->type & TYPE_INT32){
          values[i].int32 = (int32_t) signed_raw;
}
      }
    }   
    //Unsigned + Big Endian
    else if(!(sig->type & SIGNED) && sig->type & BIG_ENDIAN){
      //worry about if there are big endian sigs
    } 
    //Unsigned + Little Endian
    else if(!(sig->type & SIGNED) && !(sig->type & BIG_ENDIAN)){
      if(sig->type & TYPE_FLOAT){
        float decode;

        decode = (float)extract_bits(translated_data,sig->start_bit,size);
        decode *= sig->scale;
        decode += sig->offset;

        values[i].flt = decode;
      }
      else{
        uint64_t decode;

        decode = extract_bits(translated_data, sig->start_bit, size);
        decode *= sig->scale;
        decode += sig->offset;

        if(decode > sig->max){
          decode = sig->max;
        }
        else if(decode < sig->min){
          decode = sig->min;
        }

        if(sig->type & TYPE_INT8){
          values[i].uint8 = (uint8_t) decode;
        }
        else if(sig->type & TYPE_INT16){
          values[i].uint16 = (uint16_t) decode;
        }
        else if(sig->type & TYPE_INT24){
          values[i].uint24 = (uint32_t) decode;
        }
        else if(sig->type & TYPE_INT32){
          values[i].uint32 = (uint32_t) decode;
        }
      }
    }
  }
  ret.id = msg.id;
  ret.decoded = values;
  Store_Message(ret, dbc.sig_count);
}

CAN_Decoded_Values Get_Signal(uint16_t id, uint8_t index){
  //implement actual message
  CAN_Decoded_Values ret;
  switch(id){
    case 0x6B1:
      ret = CAN_Storage[BMS1_MSG_INDEX].decoded[index];

    default:
      ret.int8 = 0;
    }
  
    return ret;
}

CAN_Decoded_Values* Get_Message(uint16_t id){
  CAN_Decoded_Values *ret;
  //implement actual messages
  switch(id){
    case 0x6B1:
      ret = &CAN_Storage[BMS1_MSG_INDEX].decoded[0];

    default:
      ret[0].uint8 = 6;
      ret[1].uint8 = 7;
  }
  return ret;
}

/// @brief Used for CAN1 bus transmissions with queue mode 
/// @param hfdcan Pointer to CAN instance
/// @param BufferIndexes Buffers that have completed their transmission
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes){
  HAL_StatusTypeDef status;
  FDCAN_TxHeaderTypeDef header;
  CAN_Msg_Raw raw_msg;

  osMessageQueueGet(CAN1txQHandle, &raw_msg, 0, 0);

  header.Identifier = raw_msg.id;
  header.TxFrameType = FDCAN_DATA_FRAME;
  header.DataLength = raw_msg.dlc;
  header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  header.BitRateSwitch       = FDCAN_BRS_OFF;       // OFF if you're not using CAN FD bit-rate switching
  header.FDFormat            = FDCAN_CLASSIC_CAN;
  header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // unless you're using the Tx event FIFO
  header.MessageMarker       = 0;

  osSemaphoreAcquire(CAN1txSHandle, 0);
  status = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan,&header, raw_msg.data);
  osSemaphoreRelease(CAN1txSHandle);
  switch(status){
    case HAL_OK:
    break;

    case HAL_BUSY:

    default:
    Error_Handler();
    break;
  }
}

/// @brief Adds messages to CAN2 FIFO
/// @param hfdcan pointer to can
void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan){
  HAL_StatusTypeDef status;
  FDCAN_TxHeaderTypeDef header;
  CAN_Msg_Raw raw_msg;
  osMessageQueueGet(CAN2txQHandle, &raw_msg, 0, 0);
  
  header.Identifier = raw_msg.id;
  header.TxFrameType = FDCAN_DATA_FRAME;
  header.DataLength = raw_msg.dlc;
  header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  header.BitRateSwitch       = FDCAN_BRS_OFF;       // OFF if you're not using CAN FD bit-rate switching
  header.FDFormat            = FDCAN_CLASSIC_CAN;
  header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // unless you're using the Tx event FIFO
  header.MessageMarker       = 0;    

  status = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan,&header, raw_msg.data);

  switch(status){
    case HAL_OK:
    
    break;

    case HAL_BUSY:

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
    FDCAN_RxHeaderTypeDef header;
    CAN_Msg_Raw recieved;
    
    status = HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&header, recieved.data);

    switch(status){
      case HAL_OK:
      recieved.id = header.Identifier;
      recieved.dlc = header.DataLength;

      osMessageQueuePut(CAN1rxQHandle, &recieved, 0, 0);
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
    FDCAN_RxHeaderTypeDef header;
    CAN_Msg_Raw recieved;

    status = HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO1,&header, recieved.data);

    switch(status){
      case HAL_OK:
      recieved.id = header.Identifier;
      recieved.dlc = header.DataLength;

      osMessageQueuePut(CAN2rxQHandle,&recieved, 0, 0);
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

/* USER CODE END 1 */

