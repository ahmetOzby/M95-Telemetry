/*
 * Magma_Endpoint.c
 *
 *  Created on: 29 Eyl 2021
 *      Author: Ahmet
 */
#include "Magma_Endpoint.h"
#include "ST7735.h"
#include "Magma_Screen.h"
#include "Magma_Protocol_Stack.h"
#include "Proxy_Mifare.h"


void Post_Handler(char * data)
{
	switch(m95_state)
	{
		case M95_POST_CONFIG:
		{
			  ST7735_FillRectangle(0, 100, 160, 30, ST7735_BLACK);
			  m95_com.connect_received = FALSE;	//
			  M95_Post_Config(M95_UART_TIMEOUT, strlen(data), 80, 80);
			  m95_data._20_sec_ctr_u16 = 0;
			  m95_data._20_sec_timeout = START;
			  m95_state = M95_POST_DATA;
		  break;
		}

		case M95_POST_DATA:
		{
			if(m95_com.connect_received == TRUE)
			{
			  m95_com.connect_received = FALSE;
			  m95_com.ok_received = FALSE;
			  M95_Post(M95_UART_TIMEOUT, (uint8_t *)data);
			  m95_data._20_sec_ctr_u16 = 0;
			  m95_data._20_sec_timeout = START;
			  m95_state = M95_GET_DATA;
			}
			else if(m95_data._20_sec_timeout == TIME_UP)
			{
				static uint8_t time_out = 0;

				if(time_out == 0)
				{
					 m95_data._3_sec_ctr_u16 = 0;
					 m95_data._3_sec_timeout = START;
					 m95_errors.post_timeout_ctr_u32++;
					 ST7735_WriteString(10, 100, "POST TIMEOUT", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					 reader_data.nfc_session_started = FALSE;
					 if(end_point.store_failure_idx != 9)
					 {
						 end_point.stored_parameters[end_point.store_failure_idx][0] = mdb_data.product_price_u16;
						 end_point.stored_parameters[end_point.store_failure_idx][1] = mdb_data.product_num_16;
						 end_point.store_failure_idx++;
					 }
					 time_out = 1;
				}

				else if(time_out == 1 && m95_data._3_sec_timeout == TIME_UP)
				{
					m95_state = M95_SIGNAL_QUALITY_CHECK;		//POST TIMEOUT
					time_out = 0;
					return;
				}
				 return;
			}
		  break;
		}

		case M95_GET_DATA:
		{
			if(m95_com.ok_received == TRUE)
			{
				m95_com.ok_received = FALSE;
				M95_Read(M95_UART_TIMEOUT, 30);
				m95_data._3_sec_ctr_u16 = 0;
				m95_data._3_sec_timeout = START;
				m95_state = M95_END_POINT_RESPONSE;
			}

			else if(m95_data._20_sec_timeout == TIME_UP)
			{
				static uint8_t time_out = 0;

				if(time_out == 0)
				{
					m95_data._3_sec_ctr_u16 = 0;
					m95_data._3_sec_timeout = START;
					m95_errors.get_timeout_ctr_u32++;
					reader_data.nfc_session_started = FALSE;

					ST7735_WriteString(10, 100, "GET TIMEOUT", Font_7x10, ST7735_WHITE, ST7735_BLACK);
					if(end_point.store_failure_idx != 9)
					{
						end_point.stored_parameters[end_point.store_failure_idx][0] = mdb_data.product_price_u16;
						end_point.stored_parameters[end_point.store_failure_idx][1] = mdb_data.product_num_16;
						end_point.store_failure_idx++;
					}
					time_out = 1;
				}

				if(m95_data._3_sec_timeout == TIME_UP && time_out == 1)
				{
					m95_state = M95_SIGNAL_QUALITY_CHECK;		//GET TIMEOUT
					time_out = 0;
					return;
				}
			}
		  break;
		}

		case M95_END_POINT_RESPONSE:
		{
			static uint8_t function_called = 0;
			if(m95_com.ok_received == TRUE)
			{
			 if(reader_data.nfc_session_started == TRUE)
			 {

				 if(function_called == 0)
				 {
					 End_Point_Response_Handler();
					 Screen_NFC_Sale_UI();
					 function_called = 1;
				 }
			 }

			 ST7735_WriteString(10, 100, "POST SUCCESSFUL!", Font_7x10, ST7735_WHITE, ST7735_BLACK);
			 if(m95_data._3_sec_timeout == TIME_UP)
			 {
				 m95_state = M95_SIGNAL_QUALITY_CHECK;			//IDLE için
				 ST7735_FillScreen(ST7735_BLACK);
				 m95_com.ok_received = FALSE;
				 reader_data.nfc_session_started = FALSE;
				 function_called = 0;
				 end_point.begin_post = 0;
			 }
			}

			else if(m95_data._3_sec_timeout == TIME_UP)
			{
			  m95_state = M95_SIGNAL_QUALITY_CHECK;
			  function_called = 0;
			  reader_data.nfc_session_started = FALSE;
			  if(end_point.store_failure_idx != 9)
			  {
				  reader_data.nfc_session_started = FALSE;
				  end_point.stored_parameters[end_point.store_failure_idx][0] = mdb_data.product_price_u16;
				  end_point.stored_parameters[end_point.store_failure_idx][1] = mdb_data.product_num_16;
				  end_point.store_failure_idx++;
				  end_point.begin_post = 0;
			  }
			}
			break;
		}
		default: break;
	}
}


void End_Point_Response_Handler(void)
{
	uint16_t buf_start_idx = 0, buf_end_idx = 0;

	buf_start_idx = buf_end_idx + Find_String_Index("\"status\":", (char *)(m95_com.rx_buf + buf_end_idx)) + 1;
	buf_end_idx = buf_start_idx + Find_String_Index(",", (char *)(m95_com.rx_buf + buf_start_idx));


	if(m95_com.rx_buf[buf_start_idx] == '1')
	{
		end_point_data.card_status = SUCCESSFUL;
		uint8_t idx = 0;

		buf_start_idx = buf_end_idx + Find_String_Index("\"_id\":", (char *)(m95_com.rx_buf + buf_end_idx)) + 1;
		buf_end_idx = buf_start_idx + Find_String_Index(",", (char *)(m95_com.rx_buf + buf_start_idx));

		for(uint8_t i = buf_start_idx; i < buf_end_idx; i++)
		{
			end_point_data.id[idx] = m95_com.rx_buf[i];
			idx++;
		}

		idx = 0;

		buf_start_idx = buf_end_idx + Find_String_Index("\"cardID\":", (char *)(m95_com.rx_buf + buf_end_idx)) + 1;
		buf_end_idx = buf_start_idx + Find_String_Index(",", (char *)(m95_com.rx_buf + buf_start_idx));

		for(uint8_t i = buf_start_idx; i < buf_end_idx; i++)
		{
			end_point_data.card_ID[idx] = m95_com.rx_buf[i];
			idx++;
		}

		char  credit_buf[200];
		idx = 0;

		buf_start_idx = buf_end_idx + Find_String_Index("\"credit\":", (char *)(m95_com.rx_buf + buf_end_idx)) + 1;
		buf_end_idx = buf_start_idx + Find_String_Index("}", (char *)(m95_com.rx_buf + buf_start_idx));

		for(uint8_t i = buf_start_idx; i < buf_end_idx; i++)
		{
			credit_buf[idx] = m95_com.rx_buf[i];
			idx++;
		}

		end_point_data.card_credit_f32 = atof(credit_buf);
	}

	else
	{
		end_point_data.card_status = FAILED;
	}

}

void Convert_Data_2_JSON(uint16_t product_price, uint16_t product_num, char *json_data)
{
	float tl = product_price / 100.0f;
	char json_format[200];
	memset((char *)json_data, 0, strlen((char *)json_data) + 1);
	vars_to_str((char *)json_format, MGM_NEWSALE_JSON_DATA, product_num, tl);
	vars_to_str((char *)json_data, MGM_NEWSALE_JSON_PARAMETERS, strlen(json_format));
	strcat((char *)json_data, json_format);
}


void Discharge_Stored_Endpoint_Buf(void)
{
	static uint8_t buf_idx = 0;
	if(m95_state == M95_SIGNAL_QUALITY_CHECK)
	{
		Screen_Sale_UI();
		page = Post_Page;
		Convert_Data_2_JSON(end_point.stored_parameters[buf_idx][0], end_point.stored_parameters[buf_idx][1], (char *)end_point.json_data);
		m95_state = M95_POST_CONFIG;
		if(end_point.store_failure_idx > 0)
		{
			buf_idx++;
		}

		else
		{
			buf_idx = 0;
		}
		end_point.begin_post = 1;
		end_point.store_failure_idx--;
	}

	if(end_point.store_failure_idx == 0)
	{
		end_point.discharge_failure_posts = FALSE;
	}
}

