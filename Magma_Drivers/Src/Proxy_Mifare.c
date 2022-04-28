/*
 * Proxy_Mifare.c
 *
 *  Created on: Nov 11, 2021
 *      Author: Ahmet
 */
#include "Proxy_Mifare.h"

void Reader_RX_Handler(void)
{
	static uint8_t rx_buf[40];
	static uint8_t data_len_u8;
	static uint8_t command_u8;
	static uint8_t buffer_idx = 0;
	switch((uint8_t)reader_com_state)
	{
		case Reader_STX:
		{
			if(reader_com.received_byte_u8 == STX)
			{
				reader_com_state = Reader_Data_Length;
			}

			else
			{
				reader_com.stx_error_u16++;
			}
			break;
		}

		case Reader_Data_Length:
		{
			data_len_u8 = reader_com.received_byte_u8;
			if(data_len_u8 != 0)
			{
				reader_com_state = Reader_Data;
			}

			else
			{
				reader_com_state = Reader_Checksum;
			}
			break;
		}

		case Reader_Data:
		{
			rx_buf[buffer_idx] = reader_com.received_byte_u8;
			buffer_idx++;
			if(buffer_idx == data_len_u8)
			{
				reader_com_state = Reader_Checksum;
			}
			break;
		}

		case Reader_Checksum:
		{
			uint8_t check_sum_buf[40];
			check_sum_buf[0] = data_len_u8;
			check_sum_buf[1] = command_u8;
			for(uint8_t i = 0; i < data_len_u8; i++)
			{
				check_sum_buf[2 + i] = rx_buf[i];
			}
			if(Magma_Checksum_Handler(check_sum_buf, data_len_u8 + 2) == reader_com.received_byte_u8)
			{
				reader_com_state = Reader_ETX;
			}

			else
			{
				reader_com_state = Reader_STX;
				memset(rx_buf, 0, 40);
				buffer_idx = 0;
				reader_com.check_sum_error_u16++;
			}
			break;
		}

		case Reader_ETX:
		{
			if(reader_com.received_byte_u8 == ETX)
			{
				reader_data.rx.command_u8 = command_u8;
				reader_data.rx.data_len_u8 = data_len_u8;

				for(uint8_t i = 0; i < data_len_u8; i++)
				{
					reader_data.rx.data_buf[i] = rx_buf[i];
				}
				reader_com.package_ready = TRUE;
			}

			else
			{
				reader_com.etx_error_u16++;
			}

			memset(rx_buf, 0, 40);
			reader_com_state = Reader_STX;
			break;
		}
	}
}

