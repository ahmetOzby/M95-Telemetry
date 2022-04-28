/*
 * Magma_GL.h
 *
 *  Created on: Sep 16, 2021
 *      Author: Ahmet
 */

#ifndef INC_MAGMA_GL_H_
#define INC_MAGMA_GL_H_

#include "stdarg.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

#define TRUE				1
#define FALSE				0

#define START				0
#define TIME_UP				1

#define STX      			0x02
#define ETX      			0x03
#define MAX_DATA_SIZE		0x40
#define PACKAGE_OFFSET		0x04

typedef struct
{
	uint8_t _0_1_hz		: 1;
	uint8_t _1_hz 		: 1;
	uint8_t _2_hz		: 1;
	uint8_t _5_hz		: 1;
	uint8_t _10_hz 		: 1;
	uint8_t _100_hz		: 1;
	uint8_t _1000_hz	: 1;
	uint8_t reserved	: 2;
}Systick_Timer_T;

//Uart Rx Com States
enum
{
	Start_Of_Text = 0,
	Command,
	Data_Length,
	Data,
	Checksum,
	End_Of_Text
};


Systick_Timer_T sys_timer;


uint16_t Find_String_Index(char *find_this, char* find_from_this);
uint8_t Magma_Checksum_Handler(uint8_t *data, uint8_t len);
void vars_to_str(char *buffer, const char *format, ...);
int vsprintf(char *str, const char *format, va_list arg);
void ftoa(float n, char* res, int afterpoint);
int sprintf(char *str, const char *format, ...);

/*void MY_FLASH_EraseSector(void);
void MY_FLASH_SetSectorAddrs(uint8_t sector, uint32_t addrs);
void MY_FLASH_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType);
void MY_FLASH_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);*/


#endif /* INC_MAGMA_GL_H_ */
