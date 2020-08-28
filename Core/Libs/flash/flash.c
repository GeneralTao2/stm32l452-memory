/*
Library:				STM32L452 Internal FLASH read/write
Written by:			Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Edited by:			Diulgher Artiom
Last modified:	21/08/2020

Description:
							Flash library implements the following basic functionalities
								- Set sectos address
								- Flash Page Erase
								- Flash Write
								- Flash Read
								
* Copyright (C) 2019 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.								
*/

#include "flash.h"

//Private variables
//1. page start address
static uint32_t Flash_PageAddrs;
static uint16_t Flash_PageNum;

//functions definitions
//1. Erase Page
void Flash_ErasePage() {
	HAL_FLASH_Unlock();

	//Erase the required Flash page
	FLASH_EraseInitTypeDef pEraseInit;
	pEraseInit.Banks = FLASH_BANK_1;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	pEraseInit.Page = Flash_PageNum;
	pEraseInit.NbPages = 1;
	uint32_t PageError = 0xFFFFFFFF;
	HAL_FLASHEx_Erase(&pEraseInit, &PageError);

	HAL_FLASH_Lock();
}

//2. Set Page Adress
HAL_StatusTypeDef Flash_SetPageAddrs(uint16_t page, uint32_t addrs) {
	if(addrs % 8 != 0) {
		return HAL_ERROR;
	}
	Flash_PageNum = page;
	Flash_PageAddrs = addrs;
	return HAL_OK;
}

//3. Write Flash
HAL_StatusTypeDef Flash_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType) {
	if(idx % 8 != 0) {
		return HAL_ERROR;
	}
	uint32_t flashAddress = Flash_PageAddrs + idx;
	
	//Erase page before write
	Flash_ErasePage(1);
	
	//Unlock Flash
	HAL_FLASH_Unlock();
	//Write to Flash
	HAL_StatusTypeDef state;

	switch(dataType) {
		case DATA_TYPE_8: {
			uint64_t vlword = FLASH_UINT64_MAX;
			uint16_t vlword_i = 0; 							/* Counter for vlword */
			for(uint16_t i=0; i<Nsize; i++) {
				((uint8_t *)&vlword)[vlword_i] = ((uint8_t *)wrBuf)[i];
				vlword_i++;
				/* If vlword filled OR if wrBuf iterated,
				 * then we can save vlword
				 * */
				if( ((i+1) % 8 == 0) || (i == Nsize-1) ) {
					vlword_i = 0;
					state = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, vlword);
					/* If something wrong, then we leave from function with error */
					if(state != HAL_OK) {
						HAL_FLASH_Lock();
						return state;
					}
					flashAddress += 8; /* Moving the pointer to 8 bytes forward */
					vlword = FLASH_UINT64_MAX;
				}
			}
			break;
		}
		/* In case of DATA_TYPE_16 and DATA_TYPE_32 we do similar actions */
		case DATA_TYPE_16: {
			uint64_t vlword = FLASH_UINT64_MAX;
			uint16_t vlword_i = 0;
			for(uint16_t i=0; i<Nsize; i++) {
				((uint16_t *)&vlword)[vlword_i] = ((uint16_t *)wrBuf)[i];
				vlword_i++;
				if( ((i+1) % 4 == 0) || (i == Nsize-1) ) {
					vlword_i = 0;
					state = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, vlword);
					if(state != HAL_OK) {
						HAL_FLASH_Lock();
						return state;
					}
					flashAddress += 8;
					vlword = FLASH_UINT64_MAX;
				}
			}
			break;
		}
		case DATA_TYPE_32: {
			uint64_t vlword = FLASH_UINT64_MAX;
			uint16_t vlword_i = 0;
			for(uint16_t i=0; i<Nsize; i++) {
				((uint32_t *)&vlword)[vlword_i] = ((uint32_t *)wrBuf)[i];
				vlword_i++;
				if( ((i+1) % 2 == 0) || (i == Nsize-1) ) {
					vlword_i = 0;
					state = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, vlword);
					if(state != HAL_OK) {
						HAL_FLASH_Lock();
						return state;
					}
					flashAddress += 8;
					vlword = FLASH_UINT64_MAX;
				}
			}
			break;
		}
		case DATA_TYPE_64: {
			for(uint16_t i=0; i<Nsize; i++) {
				/* Here we just save uint64 by uint64 */
				state = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, ((uint64_t *)wrBuf)[i]);
				if(state != HAL_OK) {
					HAL_FLASH_Lock();
					return state;
				}
				flashAddress += 8;
			}
			break;
		}
	}

	//Lock the Flash space
	HAL_FLASH_Lock();
	return HAL_OK;
}
//4. Read Flash
HAL_StatusTypeDef Flash_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType) {
	if(idx % 8 != 0) {
			return HAL_ERROR;
	}
	uint32_t flashAddress = Flash_PageAddrs + idx;

		switch(dataType) {
			case DATA_TYPE_8:
					for(uint32_t i=0; i<Nsize; i++) {
						*((uint8_t *)rdBuf + i) = *(uint8_t *)flashAddress;
						flashAddress+=1;
					}
					break;

			case DATA_TYPE_16:
					for(uint32_t i=0; i<Nsize; i++) {
						*((uint16_t *)rdBuf + i) = *(uint16_t *)flashAddress;
						flashAddress+=2;
					}
					break;

			case DATA_TYPE_32:
					for(uint32_t i=0; i<Nsize; i++) {
						*((uint32_t *)rdBuf + i) = *(uint32_t *)flashAddress;
						flashAddress+=4;
					}
					break;
			case DATA_TYPE_64:
					for(uint32_t i=0; i<Nsize; i++) {
						*((uint64_t *)rdBuf + i) = *(uint64_t *)flashAddress;
						flashAddress+=8;
					}
					break;
		}
		return HAL_OK;
}

