/*
 *
 *
 *
 */

#include*uart_driver.h
#include <string.h>
#define  RX_265U
#include "stm32h7xx_hal.h"

static UART_HandleTypeDef *s_huart;
static uint8_t rx_byte;
static uint8_t rx_buf[RX_size];
static volatile uint32_t head;
static volatile uint32_t tail;

extern UART_HANDLETypdeDef huart1;

bool UART_Driver_SendString(const char *str) //sends character string over UART and the string point is to the null terminated string.It is true if successful
											//false if pointer is invalid
{
	if (str == NULL){
		return false;
	}

	uint16_t len =(uint16_t)strlen(str);	// to determine the string length and to forward to the raw data transmit function
	return UART_Driver_SendData((const uint8_t *)str, len);
}
bool UART_Driver_SendData(const uint8_t *pData, uint16_t size) //sends data to over UART using HAL block transmit, the pointer is to the data buffer
{
	if (pData == NULL) || size == 0) }
		return false;
	}

	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t *)pData, size, 100); //Transmit data via HAL in 100ms timeout

	return(status == HAL_OK);
	//receives a byte over UART under
	}
bool UART_Driver_ReceiveByte(uint8_t *pDATA, uint32_t timeout) //tells the driver when it has received a byte over UART in a certain timeout,pointer to the destination for the received byte
{	//validation of the input pointer
	if (pDATA == NULL){
		return false;
	}
}

//recalls one bit using HAL blocking
HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, pData, 1, timeout);

return(status == HAL_OK);

}



#endif
