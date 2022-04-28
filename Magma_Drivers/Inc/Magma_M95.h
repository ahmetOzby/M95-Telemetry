/*
 * Magma_M95.h
 *
 *  Created on: Sep 22, 2021
 *      Author: Ahmet
 */

#ifndef INC_MAGMA_M95_H_
#define INC_MAGMA_M95_H_

#include "stdint.h"
#include "string.h"
#include "Magma_GL.h"
#include "main.h"


extern UART_HandleTypeDef huart1;

#define M95_UART				huart1
#define M95_MAX_BUF_SIZE		1024
#define M95_MAX_FRAME_SIZE		1024
#define MAX_URL_SIZE			1024
#define M95_UART_TIMEOUT		200
#define CR_ASCII				0x0D
#define LF_ASCII				0x0A
#define SUCCESSFUL				1
#define FAILED					0
#define APN						"internet\0"


typedef struct
{
	uint8_t pack_received		:1;
	uint8_t ok_received			:1;
	uint8_t error_received		:1;
	uint8_t connect_received	:1;
	uint8_t com_ok				:1;
	uint8_t cme_error_rxed		:1;
	uint8_t internet_con_ok		:1;
	uint8_t end_point_con_ok	:1;
	uint8_t received_byte_u8;
	uint8_t rx_buf[M95_MAX_BUF_SIZE];
	uint8_t rx_error_u8;
	uint16_t error_ctr;
}M95_Com_t;

M95_Com_t m95_com;

typedef struct
{
	uint8_t _2_sec_timeout			: 1;
	uint8_t _3_sec_timeout 			: 1;
	uint8_t _10_sec_timeout 		: 1;
	uint8_t _12_sec_timeout 		: 1;
	uint8_t _16_sec_timeout 		: 1;
	uint8_t _20_sec_timeout			: 1;
	uint8_t _100_ms_timeout 		: 1;
	uint8_t _270_ms_timeout 		: 1;
	uint16_t _3_sec_ctr_u16;
	uint16_t _2_sec_ctr_u16;
	uint16_t _270_ms_ctr_u16;
	uint16_t _10_sec_ctr_u16;
	uint16_t _12_sec_ctr_u16;
	uint16_t _16_sec_ctr_u16;
	uint16_t _20_sec_ctr_u16;
	uint16_t _100_ms_ctr_u16;
}M95_Data_T;
M95_Data_T m95_data;

typedef struct
{
	uint32_t post_timeout_ctr_u32;
	uint32_t get_timeout_ctr_u32;
	uint8_t cme_error[100];
}M95_Errors_T;

M95_Errors_T m95_errors;





enum
{
	O_ASCII	= 'O',
	K_ASCII	= 'K',
	E_ASCII	= 'E',
	R_ASCII	= 'R',
	C_ASCII	= 'C',
	N_ASCII = 'N',
	T_ASCII = 'T'
};

enum
{
	ERROR_ASCII_STATE_1 = 0,
	ERROR_ASCII_STATE_2,
	ERROR_ASCII_STATE_3,
	ERROR_ASCII_STATE_4,
	ERROR_ASCII_STATE_5,
};

enum
{
	CONNECT_ASCII_STATE_1 = 0,
	CONNECT_ASCII_STATE_2,
	CONNECT_ASCII_STATE_3,
	CONNECT_ASCII_STATE_4,
	CONNECT_ASCII_STATE_5,
	CONNECT_ASCII_STATE_6,
	CONNECT_ASCII_STATE_7,
};

enum M95_State_Enum
{
	M95_COM_CONTROL,
	M95_ATTACH_TO_GPRS_PDP,
	M95_SELECT_GPRS_FOR_TCPIP,
	M95_SIM_CONTROL,
	M95_ACTIVATE_PDP_CONTEXT,
	M95_CHECK_NETWORK_REGISTIRITION,
	M95_SET_SSL_VERSION,
	M95_SET_SECURE_LEVEL_NO_AUTHENTICATION,
	M95_SET_CIPHER_SUITE,
	M95_SET_RTC_TIME,
	M95_SET_HTTPS,
	M95_SET_SSL_CONTEXT_FOR_HTTPS,
	M95_SET_REQUEST_HEADER_CUSTOMIZITION,
	M95_SET_HTTP_HEADER_OUTPUT,
	M95_SET_URL_CONFIG,
	M95_SET_URL,
	M95_POST_CONFIG,
	M95_POST_DATA,
	M95_GET_DATA,
	M95_GET_REQUEST,
	M95_END_POINT_RESPONSE,
	M95_TRANSMIT_ERROR,
	M95_GOOGLE_PING,
	M95_SIGNAL_QUALITY_CHECK
};

enum M95_State_Enum m95_state;


void M95_Init(void);
void M95_RX_Callback_Handler(void);
void M95_Com_Control(uint16_t timeout);
void M95_Attach_2_GPRS(uint16_t timeout, uint8_t parameter);
void M95_Select_GPRS_For_TCPIP(uint16_t timeout, uint8_t parameter, uint8_t *data);
void M95_Activate_PDP_Context(uint16_t timeout, uint8_t pdp_state, uint8_t cid);
void M95_Check_Network_Registirition(uint16_t timeout);
void M95_Set_SSL_Version(uint16_t timeout, uint8_t ctx_idx, uint8_t ssl_version);
void M95_Set_Sec_Lvl_No_Authentication(uint16_t timeout, uint8_t ctx_idx, uint8_t sec_level);
void M95_Set_Cipher_Suite(uint16_t timeout, uint8_t ctx_idx, uint8_t* cipher_suite);
void M95_Set_RTC_TIME(uint16_t timeout, uint8_t ignore_rtc_time);
void M95_Set_HTTPS(uint16_t timeout, uint8_t https_enable);
void M95_Set_SSL_Context_For_HTTPS(uint16_t timeout, uint8_t https_ctx_idx);
void M95_Set_Request_Header_Customizition(uint16_t timeout, uint8_t parameter);
void M95_Set_HTTP_Header_Output(uint16_t timeout, uint8_t parameter);
void M95_Set_URL_Config(uint16_t timeout, uint16_t url_size, uint8_t connection_timeout);
void M95_Set_URL(uint16_t timeout, uint8_t* url);
void M95_Post_Config(uint16_t timeout, uint16_t data_size, uint8_t timeout_1, uint8_t timeout_2);
void M95_Post(uint16_t timeout, uint8_t* data);
void M95_Get_Signal_Quality(uint16_t timeout);
void M95_Read(uint16_t timeout, uint8_t response_timeout);
void M95_Get(uint16_t timeout, uint8_t timeout_1);
void M95_Sim_Control(uint16_t timeout);
void M95_Ping(uint16_t timeout, char* url);


#endif /* INC_MAGMA_M95_H_ */
