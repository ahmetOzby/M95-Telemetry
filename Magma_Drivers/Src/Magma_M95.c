/*
 * Magma_M95.c
 *
 *  Created on: Sep 22, 2021
 *      Author: Ahmet
 */

#include "Magma_Endpoint.h"
#include "Magma_Screen.h"
#include "Fonts.h"
#include "ST7735.h"

void M95_RX_Callback_Handler(void)
{
	static uint8_t cr_received = FALSE, o_received = FALSE, error_ascii_state = 0, connect_ascii_state = 0;
	static uint8_t data[M95_MAX_BUF_SIZE];
	static uint16_t data_idx = 0;
	if(m95_com.received_byte_u8 == CR_ASCII)
	{
		cr_received = TRUE;
	}

	if(data_idx < M95_MAX_BUF_SIZE && cr_received == TRUE)
	{
		data[data_idx] = m95_com.received_byte_u8;
		data_idx++;
	}

	if(m95_com.received_byte_u8 == O_ASCII)
	{
		o_received = TRUE;
	}

	if(o_received == 1 && m95_com.received_byte_u8 == K_ASCII)
	{
		cr_received = FALSE;
		o_received = FALSE;
		memset(m95_com.rx_buf, 0, M95_MAX_BUF_SIZE);
		for(uint8_t i = 0; i < data_idx; i++)
		{
			m95_com.rx_buf[i] = data[i];
		}
		m95_com.pack_received = TRUE;
		m95_com.ok_received = TRUE;
		m95_com.error_received = FALSE;
		data_idx = 0;
		return;				//Eklendi denenmedi.
	}


	switch(error_ascii_state)
	{
		case ERROR_ASCII_STATE_1:
		{
			if(m95_com.received_byte_u8 == E_ASCII)
			{
				error_ascii_state = ERROR_ASCII_STATE_2;
			}
			break;
		}
		case ERROR_ASCII_STATE_2:
		{
			if(m95_com.received_byte_u8 == R_ASCII)
			{
				error_ascii_state = ERROR_ASCII_STATE_3;
			}

			else
			{
				error_ascii_state = ERROR_ASCII_STATE_1;
			}
			break;
		}
		case ERROR_ASCII_STATE_3:
		{
			if(m95_com.received_byte_u8 == R_ASCII)
			{
				error_ascii_state = ERROR_ASCII_STATE_4;
			}

			else
			{
				error_ascii_state = ERROR_ASCII_STATE_1;
			}
			break;
		}
		case ERROR_ASCII_STATE_4:
		{
			if(m95_com.received_byte_u8 == O_ASCII)
			{
				error_ascii_state = ERROR_ASCII_STATE_5;
			}

			else
			{
				error_ascii_state = ERROR_ASCII_STATE_1;
			}
			break;
		}
		case ERROR_ASCII_STATE_5:
		{
			if(m95_com.received_byte_u8 == R_ASCII)
			{
				cr_received = FALSE;
				o_received = FALSE;
				memset(m95_com.rx_buf, 0, M95_MAX_BUF_SIZE);
				for(uint8_t i = 0; i < data_idx; i++)
				{
					m95_com.rx_buf[i] = data[i];
				}
				m95_com.pack_received = TRUE;
				//m95_com.ok_received = TRUE;
				m95_com.error_received = TRUE;
				m95_com.error_ctr++;
				data_idx = 0;
				error_ascii_state = ERROR_ASCII_STATE_1;
			}
			return;		//Eklendi denenmedi.
			break;
		}
	}

		switch(connect_ascii_state)
		{
			case CONNECT_ASCII_STATE_1:
			{
				if(m95_com.received_byte_u8 == C_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_2;
				}
				break;
			}
			case CONNECT_ASCII_STATE_2:
			{
				if(m95_com.received_byte_u8 == O_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_3;
				}

				else
				{
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				break;
			}
			case CONNECT_ASCII_STATE_3:
			{
				if(m95_com.received_byte_u8 == N_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_4;
				}

				else
				{
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				break;
			}
			case CONNECT_ASCII_STATE_4:
			{
				if(m95_com.received_byte_u8 == N_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_5;
				}

				else
				{
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				break;
			}
			case CONNECT_ASCII_STATE_5:
			{
				if(m95_com.received_byte_u8 == E_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_6;
				}

				else
				{
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				break;
			}

			case CONNECT_ASCII_STATE_6:
			{
				if(m95_com.received_byte_u8 == C_ASCII)
				{
					connect_ascii_state = CONNECT_ASCII_STATE_7;
				}

				else
				{
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				break;
			}

			case CONNECT_ASCII_STATE_7:
			{
				if(m95_com.received_byte_u8 == T_ASCII)
				{
					cr_received = FALSE;
					o_received = FALSE;
					memset(m95_com.rx_buf, 0, M95_MAX_BUF_SIZE);
					for(uint8_t i = 0; i < data_idx; i++)
					{
						m95_com.rx_buf[i] = data[i];
					}
					m95_com.pack_received = TRUE;
					//m95_com.ok_received = TRUE;
					m95_com.connect_received = TRUE;
					m95_com.error_received = FALSE;
					data_idx = 0;
					connect_ascii_state = CONNECT_ASCII_STATE_1;
				}
				return;		//Eklendi Denenmedi.
				break;
			}

		default:	break;
	}
}

void M95_Com_Control(uint16_t timeout)
{

	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)"AT\r\n", strlen("AT\r\n\0"), (timeout)) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Attach_2_GPRS(uint16_t timeout, uint8_t parameter)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+CGATT=%u\r\n\0", parameter);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Sim_Control(uint16_t timeout)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+CPIN?\r\n\0");
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Select_GPRS_For_TCPIP(uint16_t timeout, uint8_t parameter, uint8_t *data)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QICSGP=%u\r\n\0", parameter);
	strcat((char *)data, "\r\n\0");
	strcat((char *)frame, (const char *)data);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Activate_PDP_Context(uint16_t timeout, uint8_t pdp_state, uint8_t cid)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+CGACT=%u,%u\r\n\0", pdp_state, cid);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Check_Network_Registirition(uint16_t timeout)
{
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)"AT+CGREG?\r\n", strlen("AT+CGREG?\r\n\0"), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_SSL_Version(uint16_t timeout, uint8_t ctx_idx, uint8_t ssl_version)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"sslversion\",%u,%u\r\n\0", ctx_idx, ssl_version);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_Sec_Lvl_No_Authentication(uint16_t timeout, uint8_t ctx_idx, uint8_t sec_level)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"seclevel\",%u,%u\r\n\0", ctx_idx, sec_level);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_Cipher_Suite(uint16_t timeout, uint8_t ctx_idx, uint8_t* cipher_suite)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"ciphersuite\",%u,", ctx_idx);
	strcat((char *)cipher_suite, "\r\n\0");
	strcat((char *)frame, (const char *)cipher_suite);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_RTC_TIME(uint16_t timeout, uint8_t ignore_rtc_time)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"ignorertctime\",%u\r\n", ignore_rtc_time);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_HTTPS(uint16_t timeout, uint8_t https_enable)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"https\",%u\r\n", https_enable);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_SSL_Context_For_HTTPS(uint16_t timeout, uint8_t https_ctx_idx)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QSSLCFG=\"httpsctxi\",%u\r\n", https_ctx_idx);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_Request_Header_Customizition(uint16_t timeout, uint8_t parameter)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPCFG=\"requestheader\",%u\r\n", parameter);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_HTTP_Header_Output(uint16_t timeout, uint8_t parameter)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPCFG=\"responseheader\",%u\r\n", parameter);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_URL_Config(uint16_t timeout, uint16_t url_size, uint8_t connection_timeout)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPURL=%u,%u\r\n", url_size, connection_timeout);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}
void M95_Set_URL(uint16_t timeout, uint8_t* url)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	strcpy((char *)frame, (const char *)url);
	strcat((char *)frame, "\r\n\0");
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Post_Config(uint16_t timeout, uint16_t data_size, uint8_t timeout_1, uint8_t timeout_2)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPPOST=%u,%u,%u\r\n", data_size, timeout_1, timeout_2);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Get(uint16_t timeout, uint8_t timeout_1)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPGET=%u\r\n", timeout_1);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Post(uint16_t timeout, uint8_t* data)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	strcpy((char *)frame, (const char *)data);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Get_Signal_Quality(uint16_t timeout)
{
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)"AT+CSQ\r\n\0", strlen("AT+CSQ\r\n\0"), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Read(uint16_t timeout, uint8_t response_timeout)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QHTTPREAD=%u\r\n\0", response_timeout);
	m95_com.pack_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, (uint8_t *)frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Ping(uint16_t timeout, char* url)
{
	uint8_t frame[M95_MAX_FRAME_SIZE];
	vars_to_str((char *)frame, "AT+QPING=\"%s\"\r\n\0", url);
	m95_com.pack_received = FALSE;
	m95_com.ok_received = FALSE;
	while(HAL_UART_Transmit(&M95_UART, frame, strlen((char *)frame), timeout) != HAL_OK)
	{
		m95_state = M95_TRANSMIT_ERROR;
	}
}

void M95_Init(void)
{
	m95_com.error_received = FALSE;
	m95_com.error_ctr = 0;
	m95_com.pack_received = FALSE;
	Screen_Opening();
	ST7735_WriteString(20, 110, "GSM INITIALIZATION", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	while(m95_state == M95_COM_CONTROL)
	{
		if(m95_com.ok_received == TRUE)
		{
			m95_state = M95_SIM_CONTROL;
			m95_com.pack_received = TRUE; //Main Init dongusune girebilmesi için (Alt kisimdaki "while dongusu")
			m95_com.ok_received = FALSE;
			HAL_Delay(1200);
		}

		if(m95_com.pack_received == FALSE)
		{
			HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
			HAL_Delay(600);
			Screen_Opening();
			HAL_Delay(600);
			Screen_Opening();
			HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
			HAL_Delay(600);
			for(uint16_t i = 0; i < 15; i++)
			{
				M95_Com_Control(M95_UART_TIMEOUT);
				HAL_Delay(300);
				Screen_Opening();
				HAL_Delay(300);
				Screen_Opening();
				HAL_Delay(500);
				Screen_Opening();
			}
		}
	}

	//Main Init
	while(m95_state != M95_SIGNAL_QUALITY_CHECK)
	{
		if(m95_com.pack_received == TRUE)
		{

			switch(m95_state)
			{
				case M95_SIM_CONTROL:
				{
					static uint8_t flag = 0;
					static uint8_t ctr = 0;
					ctr++;
					if(flag == 0 || ctr > 10)
					{
						Screen_Opening();
						HAL_Delay(300);
						Screen_Opening();
						HAL_Delay(300);
						Screen_Opening();
						ST7735_WriteString(10, 110, "SIM CONTROL                            ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						M95_Sim_Control(100);
						ctr = 0;
						flag = 1;
					}

					if(flag == 1)
					{
						HAL_Delay(500);
						if(Find_String_Index("READY", (char *)m95_com.rx_buf) != 0 && m95_com.ok_received == TRUE)
						{
							flag = 0;
							m95_state = M95_ATTACH_TO_GPRS_PDP;
							m95_com.ok_received = FALSE;
						}

						else
						{
							HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
							HAL_Delay(600);
							Screen_Opening();
							HAL_Delay(600);
							Screen_Opening();
							HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
							m95_com.ok_received = FALSE;
							while(m95_com.ok_received != TRUE)
							{
								HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
								HAL_Delay(600);
								Screen_Opening();
								HAL_Delay(600);
								Screen_Opening();
								HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
								for(uint16_t i = 0; i < 15; i++)
								{
									M95_Com_Control(M95_UART_TIMEOUT);
									HAL_Delay(300);
									Screen_Opening();
									HAL_Delay(300);
									Screen_Opening();
									HAL_Delay(500);
									Screen_Opening();
								}
							}
							flag = 0;
						}

					}

					break;
				}

				case M95_ATTACH_TO_GPRS_PDP:
				{
					ST7735_WriteString(10, 110, "ATTACH_TO_GPRS_PDP                              ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Attach_2_GPRS(M95_UART_TIMEOUT, 1);
					HAL_Delay(275);
					Screen_Opening();
					HAL_Delay(275);
					Screen_Opening();
					HAL_Delay(275);
					Screen_Opening();
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SELECT_GPRS_FOR_TCPIP;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SELECT_GPRS_FOR_TCPIP:
				{
					ST7735_WriteString(10, 110, "SELECT_GPRS_FOR_TCPIP                        ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Select_GPRS_For_TCPIP(M95_UART_TIMEOUT, 1, (uint8_t *)"internet");
					Screen_Opening();
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_ACTIVATE_PDP_CONTEXT;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_ACTIVATE_PDP_CONTEXT:
				{
					ST7735_WriteString(10, 110, "ACTIVATE_PDP_CONTEXT                        ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Activate_PDP_Context(M95_UART_TIMEOUT, 1, 1);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						//m95_state = M95_CHECK_NETWORK_REGISTIRITION;
						m95_state = M95_SET_SSL_VERSION;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_SSL_VERSION:
				{
					ST7735_WriteString(10, 110, "SET_SSL_VERSION                           ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_SSL_Version(M95_UART_TIMEOUT, 1, 3);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_SECURE_LEVEL_NO_AUTHENTICATION;
						m95_com.ok_received = FALSE;
					}

					else if(m95_com.error_ctr > 10 && m95_com.error_received == TRUE)
					{
						m95_state = M95_COM_CONTROL;
						page = Idle_Page;
						M95_Init();
					}
					break;
				}

				case M95_SET_SECURE_LEVEL_NO_AUTHENTICATION:
				{
					ST7735_WriteString(10, 110, "SET_SECURITY                          ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_Sec_Lvl_No_Authentication(M95_UART_TIMEOUT, 1, 0);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_CIPHER_SUITE;
						m95_com.ok_received = FALSE;
					}

					else if(m95_com.error_received == TRUE)
					{
						HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
						HAL_Delay(600);
						HAL_GPIO_TogglePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin);
						M95_Init();
						return;
					}
					break;
				}

				case M95_SET_CIPHER_SUITE:
				{
					ST7735_WriteString(10, 110, "SET_CIPHER_SUITE                       ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					uint8_t cipher_suite[9] = "\"0X003D\"\0";
					M95_Set_Cipher_Suite(M95_UART_TIMEOUT, 1, cipher_suite);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_RTC_TIME;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_RTC_TIME:
				{
					ST7735_WriteString(10, 110, "SET_RTC_TIME                            ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_RTC_TIME(M95_UART_TIMEOUT, 1);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_HTTPS;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_HTTPS:
				{
					ST7735_WriteString(10, 110, "SET_HTTPS	                           ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_HTTPS(M95_UART_TIMEOUT, 1);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_SSL_CONTEXT_FOR_HTTPS;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_SSL_CONTEXT_FOR_HTTPS:
				{
					ST7735_WriteString(10, 110, "SET_SSL_CONTEXT	                    ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_SSL_Context_For_HTTPS(M95_UART_TIMEOUT, 1);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_REQUEST_HEADER_CUSTOMIZITION;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_REQUEST_HEADER_CUSTOMIZITION:
				{
					ST7735_WriteString(10, 110, "CUSTOMIZE_HEADER	                      ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_Request_Header_Customizition(M95_UART_TIMEOUT, 1);		//1
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_HTTP_HEADER_OUTPUT;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_HTTP_HEADER_OUTPUT:
				{
					ST7735_WriteString(10, 110, "SET_HTTP_HEADER_OUTPUT", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_HTTP_Header_Output(M95_UART_TIMEOUT, 0);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SET_URL_CONFIG;
						m95_com.ok_received = FALSE;
					}
					break;
				}

				case M95_SET_URL_CONFIG:
				{
					ST7735_WriteString(10, 110, "SET_URL_CONFIG                             ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					uint8_t url[MAX_URL_SIZE] = MAGMA_URL;
					strcpy((char*)end_point.url, MAGMA_NFC_URL);
					M95_Set_URL_Config(M95_UART_TIMEOUT, strlen((char *)url), 60);
					HAL_Delay(275);
					Screen_Opening();

					if(m95_com.connect_received == TRUE)
					{
						m95_state = M95_SET_URL;
						m95_com.connect_received = FALSE;
						m95_com.end_point_con_ok = TRUE;
						m95_com.cme_error_rxed = FALSE;
					}

					else if(m95_com.error_received == TRUE)
					{
						Find_String_Index("+CME ERROR:", (char *)m95_com.rx_buf);
						m95_com.cme_error_rxed = TRUE;
						m95_com.end_point_con_ok = FALSE;
					}
					break;
				}

				case M95_SET_URL:
				{
					uint8_t arr[200] = MAGMA_URL;
					ST7735_WriteString(10, 110, "SET_URL		                             ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					M95_Set_URL(M95_UART_TIMEOUT, arr);
					HAL_Delay(275);
					Screen_Opening();
					if(m95_com.ok_received == TRUE)
					{
						m95_state = M95_SIGNAL_QUALITY_CHECK;
						m95_com.ok_received = FALSE;
						m95_url = CASHLESS_POST_URL;
					}
					m95_com.internet_con_ok = TRUE;
					screen_data.internet_con_changed = TRUE;						//Ana ekrana gecis icin.
					break;
				}
				default: break;
			}
		}
	}
}

