/*
 * dma_usart.h
 *
 *  Created on: Jul 13, 2020
 *      Author: ar725
 */

#ifndef LIBS_DMA_USART_DMA_USART_H_
#define LIBS_DMA_USART_DMA_USART_H_

#include "../tools/tools.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
 * To use this lib you need to do some things:
 * 1) Activate USRT2 with Asynchronous mode;
 * 2) Add TX channel of USRT2 to DMA with default settings.
 * Don't forget to call DMA_USART_Init.
 */

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;

#define DMA_USART_MESSAGE_LENGTH 512

/* Buffer for text storing */
static uint8_t mes[DMA_USART_MESSAGE_LENGTH];

void DMA_USART_Printf(const char * format, ... );

void DMA_USART_Print();

void DMA_USART_Init(void);

void DMATransferComplete(DMA_HandleTypeDef *hdma);

uint8_t* DMA_USART_GetBuffer();

void DMA_USART_ClearBuffer();

#endif /* LIBS_DMA_USART_DMA_USART_H_ */
