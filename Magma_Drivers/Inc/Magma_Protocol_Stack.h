/*
 * Magma_Protocol_Stack.h
 *
 *  Created on: 17 Eyl 2021
 *      Author: Ahmet
 */

#ifndef INC_MAGMA_PROTOCOL_STACK_H_
#define INC_MAGMA_PROTOCOL_STACK_H_

#include "Magma_GL.h"
#include "stdlib.h"
#include "main.h"
#include "string.h"
#include "Magma_Typecast.h"
#include "Magma_Screen.h"

#define MDB_UART					huart2
#define MAX_PS_PACKAGE_SIZE			20
#define MAX_PS_DATA_SIZE			16
#define MAX_MDB_DATA_SIZE			16

#define MDB_MAX_RESPONSE_TIME		0x05


typedef enum {
	VendState_BeginSession = 0,
	VendState_Denied,
	VendState_Failure,
	VendState_Success,
	VendState_VendRequested,
	VendState_Inactive,
	VendState_Disable,
	VendState_SessionCompleted,
	VendState_Cancel,
	VendState_Approved,
	VendState_Idle,
	VendState_Enable,
	VendState_Vend,
	VendState_SessionIdle,
	VendState_Cancelled,
	VendState_CommunicationFailure, // Arduino not communication with VMC
	VendState_RevalueRequested,
	VendState_Revalue
}VendState;
VendState t_vend_state;


typedef struct
{
	uint8_t begin_session_started 	: 1;
	uint8_t product_selected		: 1;
	uint8_t rx_buffer_ready 		: 1;
	uint8_t vend_request	 		: 1;
	uint8_t ack_rxed				: 1;
	uint8_t reserved 				: 3;

	struct
	{
		uint8_t command_u8;
		uint8_t data_len_u8;
		uint8_t package_len_u8;
		uint8_t data[MAX_PS_DATA_SIZE];
		uint8_t checksum_u8;
		uint8_t package[MAX_PS_PACKAGE_SIZE];
	}tx;

	struct
	{
		uint8_t received_byte_u8;
		uint8_t com_state_u8;
		uint8_t data_len_u8;
		uint8_t command_u8;
		uint8_t stx_error_ctr_u8;
		uint8_t checksum_error_ctr_u8;
		uint8_t etx_error_ctr_u8;
		uint8_t data_buf[MAX_MDB_DATA_SIZE];
	}rx;
}MDB_Com_T;

MDB_Com_T mdb_com;

typedef struct
{
	uint8_t user_funds_h_u8;
	uint8_t user_funds_l_u8;
	uint8_t error_message_u8;

	uint16_t product_price_u16;
	uint16_t product_num_16;
	uint8_t _30sec_ctr_u16;
	uint8_t _30sec_cancel_ctr_u16;

	uint8_t _30sec_timeout 	: 1;
	uint8_t _30sec_cancel_timeout;
	uint8_t reserved		: 7;
}MDB_Data_T;

MDB_Data_T mdb_data;


//PS Tx Command
enum
{
	Vend_Poll = 7,
	Begin_Session,
	Vend_Request,
	Vend_Cancelled,
	Vend_Denied,
	Vend_Approved,
	Cash_Sale,
	Telemetry_ACK
};


//Command Data Size
enum
{
	Poll_Data_Size = 0,
	Begin_Session_Data_Size,
	Vend_Request_Data_Size,
	Vend_Cancelled_Data_Size,
	Vend_Denied_Data_Size,
	Vend_Approved_Data_Size,
	Cash_Sale_Data_Size = 0
};


//Error States
enum
{
	MDB_Com_OK = 0,
	MDB_Com_Error = 0x01,
	MDB_X_Error = 0x02,
	MDB_Y_Error = 0x04,
	MDB_Z_Error = 0x08
};

void Send_MDB(uint8_t command, uint8_t *data, uint8_t data_len);
void Begin_Session_Handler(uint8_t user_funds_h, uint8_t user_funds_l);
void Vend_Request_Handler(void);
void Vend_Cancelled_Handler(void);
void Vend_Denied_Handler(void);
void Vend_Approved_Handler(void);
void Vend_Cash_Sale_Handler(void);
void NFC_Card_Session(uint8_t* card_uid, char *json_data);
void MDB_RX_Handler(void);
#endif /* INC_MAGMA_PROTOCOL_STACK_H_ */
