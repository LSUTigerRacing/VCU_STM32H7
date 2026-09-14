/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INLET_TEMPERATURE_Pin GPIO_PIN_1
#define INLET_TEMPERATURE_GPIO_Port GPIOC
#define OUTLET_TEMPERATURE_Pin GPIO_PIN_2
#define OUTLET_TEMPERATURE_GPIO_Port GPIOA
#define FRONT_BRAKE_PRESSURE_Pin GPIO_PIN_3
#define FRONT_BRAKE_PRESSURE_GPIO_Port GPIOA
#define BACK_BRAKE_PRESSURE_Pin GPIO_PIN_5
#define BACK_BRAKE_PRESSURE_GPIO_Port GPIOA
#define SUSPENSION_FL_Pin GPIO_PIN_6
#define SUSPENSION_FL_GPIO_Port GPIOA
#define SUSPENSION_BR_Pin GPIO_PIN_7
#define SUSPENSION_BR_GPIO_Port GPIOA
#define SUSPENSION_FR_Pin GPIO_PIN_4
#define SUSPENSION_FR_GPIO_Port GPIOC
#define STEERING_ANGLE_Pin GPIO_PIN_5
#define STEERING_ANGLE_GPIO_Port GPIOC
#define THROTTLE_PEDAL_POSITION_Pin GPIO_PIN_0
#define THROTTLE_PEDAL_POSITION_GPIO_Port GPIOB
#define SUSPENSION_BL_Pin GPIO_PIN_1
#define SUSPENSION_BL_GPIO_Port GPIOB
#define SD_Detect_Pin GPIO_PIN_14
#define SD_Detect_GPIO_Port GPIOD
#define DigOut_5_Pin GPIO_PIN_15
#define DigOut_5_GPIO_Port GPIOD
#define DigOut_4_Pin GPIO_PIN_6
#define DigOut_4_GPIO_Port GPIOC
#define DigOut_3_Pin GPIO_PIN_8
#define DigOut_3_GPIO_Port GPIOA
#define DigOut_2_Pin GPIO_PIN_9
#define DigOut_2_GPIO_Port GPIOA
#define DigOut_1_Pin GPIO_PIN_10
#define DigOut_1_GPIO_Port GPIOA
#define DigIn_12_Pin GPIO_PIN_0
#define DigIn_12_GPIO_Port GPIOD
#define DigIn_10_Pin GPIO_PIN_1
#define DigIn_10_GPIO_Port GPIOD
#define DigIn_8_Pin GPIO_PIN_7
#define DigIn_8_GPIO_Port GPIOD
#define DigIn_7_Pin GPIO_PIN_5
#define DigIn_7_GPIO_Port GPIOB
#define DigIn_6_Pin GPIO_PIN_6
#define DigIn_6_GPIO_Port GPIOB
#define DigIn_5_Pin GPIO_PIN_7
#define DigIn_5_GPIO_Port GPIOB
#define DigIn_4_Pin GPIO_PIN_8
#define DigIn_4_GPIO_Port GPIOB
#define DigIn_3_Pin GPIO_PIN_9
#define DigIn_3_GPIO_Port GPIOB
#define DigIn_2_Pin GPIO_PIN_0
#define DigIn_2_GPIO_Port GPIOE
#define DigIn_1_Pin GPIO_PIN_1
#define DigIn_1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
