/*
 *
 * Author: Andrew Richardson
 *
 * Created: 5 October 2026
 *
 *
 */

#ifndef UAR_DRIVER_H
#define UAR_DRIVER_H

#include "main.h"
#include "stm32h7xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//initializes UART communication

void UART_Driver_Init(void);

// sends null terminated string via UART

bool UART_Driver_SendString(const char str);

// sends the data over the UART buffer

bool UART_Driver_SendData(const uint8_t *pDATA, uint16_t size);

//receives the UART bytes under a specific timeout

bool UART_Driver_ReceivedByte(const uint8_t *pData, uint32 timeout);

#endif
