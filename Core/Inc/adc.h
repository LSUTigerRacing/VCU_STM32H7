/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc2;

extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */

/* BEGIN Defines*/
#define D2_RAM __attribute__((section(".D2_RAM")))

#define ADC12_BUFFER_COUNT 10
#define ADC3_BUFFER_COUNT 4

// Suspention (Front/Back Left/Right) shock travel potentiometer pins
#define SUS_FL 0
#define SUS_FR 1
#define SUS_BL 2
#define SUS_BR 3

// Steering angle potentiometer pin
#define STEERING 4

// Throttle pedal position potentiometer pin
#define THROTTLE 5

// Radiator inlet/outlet temp pin
#define INLET 6
#define OUTLET 7

// Brake pressure pin
#define F_BRAKE 8
#define B_BRAKE 9

/* END Defines*/

/* BEGIN Extern */
extern volatile D2_RAM uint32_t adc12_dma_buf[ADC12_BUFFER_COUNT];
extern volatile D2_RAM uint32_t adc3_dma_buf[ADC3_BUFFER_COUNT];

typedef struct {
  uint32_t data1;
  uint32_t data2;
  uint32_t avg;
} ADC_Data;

// Suspension shock travel
extern volatile ADC_Data sus_fl;
extern volatile ADC_Data sus_fr;
extern volatile ADC_Data sus_bl;
extern volatile ADC_Data sus_br;

// Steering angle
extern volatile ADC_Data steering_ang;

// Throttle pedal position
extern volatile ADC_Data throttle_pos;

// Radiator inlet/outlet temperature
extern volatile ADC_Data inlet_temp;
extern volatile ADC_Data outlet_temp;

// Front/Back brake pressure
extern volatile ADC_Data f_brake_press;
extern volatile ADC_Data b_brake_press;

/* END Extern*/
/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

