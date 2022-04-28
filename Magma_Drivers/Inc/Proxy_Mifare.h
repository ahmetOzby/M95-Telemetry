/*
 * Proxy_Mifare.h
 *
 *  Created on: Nov 11, 2021
 *      Author: Ahmet
 */

#ifndef INC_PROXY_MIFARE_H_
#define INC_PROXY_MIFARE_H_

#include "Magma_GL.h"

#define NFC_UART	huart4

typedef struct
{
	uint8_t package_ready 	: 1;
	uint8_t reserved		: 7;
	uint8_t received_byte_u8;
	uint8_t rx_buf[40];
	uint16_t stx_error_u16;
	uint16_t check_sum_error_u16;
	uint16_t etx_error_u16;
}Reader_Com_T;

Reader_Com_T reader_com;

typedef struct
{
	struct
	{

	}tx;

	struct
	{
		uint8_t command_u8;
		uint8_t data_len_u8;
		uint8_t data_buf[40];
	}rx;

	uint8_t nfc_card_read_flag		: 1;
	uint8_t nfc_session_started		: 1;
	uint8_t reserved				: 6;
}Reader_Data_T;

Reader_Data_T reader_data;

typedef enum
{
	Reader_STX,
	Reader_Data_Length,
	Reader_Command,
	Reader_Data,
	Reader_Checksum,
	Reader_ETX
}Reader_Com_State_T;

Reader_Com_State_T reader_com_state;

void Reader_RX_Handler(void);

#endif /* INC_PROXY_MIFARE_H_ */
