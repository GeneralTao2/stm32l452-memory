/*
 * dma_usart.c
 *
 *  Created on: Jul 13, 2020
 *      Author: ar725
 */
#include "../dma_usart/dma_usart.h"

/* Printing text in printf style */
void DMA_USART_Printf(const char * format, ... ) {
	/* Custom sprintf implementation */
	va_list args;
	va_start(args, format);
	vsprintf((char*)mes, format, args);
	va_end(args);

	DMA_USART_Print();
}

/* Printing already loaded mes */
void DMA_USART_Print() {
	/* Enabling DMA transmission bit */
	huart2.Instance->CR3 |= USART_CR3_DMAT;

	/* Starting USART to DMA transmission interrupt */
	HAL_DMA_Start_IT(&hdma_usart2_tx, (uint32_t)mes,
				(uint32_t)&huart2.Instance->TDR, strlen((char*)mes));
}

/*! Initializing DMA USART */
void DMA_USART_Init(void) {
  HAL_DMA_RegisterCallback(&hdma_usart2_tx, HAL_DMA_XFER_CPLT_CB_ID, &DMATransferComplete);
}

/* Callback function for disabling USART DMA mode */
void DMATransferComplete(DMA_HandleTypeDef *hdma) {
	huart2.Instance->CR3 &= ~USART_CR3_DMAT;
}

uint8_t* DMA_USART_GetBuffer() {
	return mes;
}

void DMA_USART_ClearBuffer() {
	for(uint16_t i=0; i<DMA_USART_MESSAGE_LENGTH; i++) {
		mes[i] = 0;
	}
}
