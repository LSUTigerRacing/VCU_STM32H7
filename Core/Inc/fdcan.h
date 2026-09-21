/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan1;

extern FDCAN_HandleTypeDef hfdcan2;

extern FDCAN_HandleTypeDef hfdcan3;

/* USER CODE BEGIN Private defines */
#define BUFFERSIZE  5
/* USER CODE END Private defines */

void MX_FDCAN1_Init(void);
void MX_FDCAN2_Init(void);
void MX_FDCAN3_Init(void);

/* USER CODE BEGIN Prototypes */
typedef union{
  uint8_t uint8;
  int8_t int8;
  uint16_t uint16;
  int16_t int16;
  uint32_t uint24; //non standard type
  int32_t int24; //non standard type
  uint32_t uint32;
  int32_t int32;
  float flt;
}CAN_Decoded_Values;

typedef struct{
  uint8_t start_bit;
  uint8_t bit_length;
  float scale;
  int16_t offset;
  float min;
  float max;
  uint8_t type;
}CAN_Signal;

typedef struct{
  uint16_t id;
  uint8_t sig_count;
  CAN_Signal *signal;
} DBC_Translation;

typedef struct{
  uint16_t id;
  uint8_t dlc;
  uint8_t data[8];
} CAN_Msg_Raw;

void Prepare_Message(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t dlc);
void Decode_Message(CAN_Msg_Raw msg, DBC_Translation dbc);

DBC_Translation Assign_Signal(CAN_Msg_Raw *msg);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

