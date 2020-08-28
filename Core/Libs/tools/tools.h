/*
 * tools.h
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: ar725
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include "stm32l4xx_hal.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define WORD_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN
#define WORD_TO_BINARY(byte) BYTE_TO_BINARY(byte>>8), BYTE_TO_BINARY(byte)

#define DWORD_TO_BINARY_PATTERN WORD_TO_BINARY_PATTERN WORD_TO_BINARY_PATTERN
#define DWORD_TO_BINARY(byte) WORD_TO_BINARY(byte>>16), WORD_TO_BINARY(byte)

typedef struct GPIO_PinConfigs {
	GPIO_TypeDef *GPIO;
	uint16_t PIN;
} GPIO_PinConfigs;

#endif /* SRC_TOOLS_H_ */
