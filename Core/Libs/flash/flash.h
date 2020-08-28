/*
Library:				STM32L452 Internal FLASH read/write
Written by:			Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Edited by:			Diulgher Artiom
Last modified:	21/08/2020

Description:
							Flash library implements the following basic functionalities
								- Set sectos address
								- Flash Sector Erase
								- Flash Write
								- Flash Read
								
* Copyright (C) 2019 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.								
*/

#include "../tools/tools.h"

#define FLASH_UINT64_MAX 0xFFFFFFFFFFFFFFFF

//Typedefs
//1. data size
typedef enum
{
	DATA_TYPE_8=0,
	DATA_TYPE_16,
	DATA_TYPE_32,
	DATA_TYPE_64,
}DataTypeDef;

//functions prototypes
//1. Erase Page
void Flash_ErasePage();

//2. Set Page Adress
HAL_StatusTypeDef Flash_SetPageAddrs(uint16_t page, uint32_t addrs);
//3. Write Flash
HAL_StatusTypeDef Flash_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType);
//4. Read Flash
HAL_StatusTypeDef Flash_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);


