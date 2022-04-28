/*
 * Magma_Screen.h
 *
 *  Created on: 7 Eki 2021
 *      Author: Ahmet
 */
#ifndef INC_MAGMA_SCREEN_H_
#define INC_MAGMA_SCREEN_H_

#include "stdint.h"

typedef enum
{
	Intro_Page = 1,
	Idle_Page,
	Post_Page,
	NFC_Post_Page
}Screen_Page_Enum;
Screen_Page_Enum page;

typedef struct
{
	uint8_t internet_con_changed 	: 1;
	uint8_t reserved				: 7;
}Screen_Data_t;

Screen_Data_t screen_data;


void Set_Screen_Rotation(uint8_t rotation);
void Screen_Opening(void);
void Screen_Sale_UI(void);
void Screen_Idle_Page(uint8_t sq, uint8_t internet_connection);
void Screen_Show_Error_Message(char* error_code);
void Screen_Signal_Quality(uint8_t x, uint8_t y, uint16_t color, uint8_t state);
void Screen_NFC_Sale_UI(void);

#endif /* INC_MAGMA_SCREEN_H_ */
