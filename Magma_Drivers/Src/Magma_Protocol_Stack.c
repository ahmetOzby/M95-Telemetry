/*
 * Magma_Protocol_Stack.c
 *
 *  Created on: 17 Eyl 2021
 *      Author: Ahmet
 */
#include "Magma_Protocol_Stack.h"
#include "Magma_Endpoint.h"
#include "string.h"
#include "Magma_GL.h"

extern UART_HandleTypeDef huart2;
extern uint8_t post_test_flag;
extern uint8_t rfid_begin_session_test_flag;

void Send_MDB(uint8_t command, uint8_t *data, uint8_t data_len)
{
	mdb_com.tx.package_len_u8 = data_len + 1 + PACKAGE_OFFSET;	//Paket boyutu hesaplanırken veri uzunluğu, komut ve paket parametreleri toplanır.

	mdb_com.tx.package[0] = STX;
	mdb_com.tx.package[1] = command;
	mdb_com.tx.package[2] = data_len;

	for(uint8_t i = 0; i < data_len; i++)
	{
		mdb_com.tx.package[i + 3] = data[i];
	}

	mdb_com.tx.checksum_u8 = Magma_Checksum_Handler(mdb_com.tx.package + 1, data_len + 3);

	mdb_com.tx.package[data_len + 3] = mdb_com.tx.checksum_u8;
	mdb_com.tx.package[data_len + 4] = ETX;

	HAL_UART_Transmit(&MDB_UART, mdb_com.tx.package, mdb_com.tx.package_len_u8, HAL_MAX_DELAY);
}

void Begin_Session_Handler(uint8_t user_funds_h, uint8_t user_funds_l)
{
	//TODO Serverdan gelen cevaba göre cashless ile konusulacak
	mdb_com.tx.data_len_u8 = 2;
	mdb_com.tx.data[0] = user_funds_h;
	mdb_com.tx.data[1] = user_funds_l;

	Send_MDB(Begin_Session, mdb_com.tx.data, mdb_com.tx.data_len_u8);
/*
	mdb_com.begin_session_started = TRUE;
	mdb_data._30sec_ctr_u16 = 0;
	mdb_data._30sec_timeout = START;*/
}

void Vend_Request_Handler(void)
{
	//TODO Otomattan seçim yapıldığında MDB'den gelecek flag'e göre Endpoint'e istek atılacak.
	//mdb_com.tx.data_len_u8 = 0;
	//PS_Send(Vend_Request, mdb_com.tx.data, mdb_com.tx.data_len_u8);

	//TODO Yeterli bakiye var mı varsa vend approved yoksa denied
	uint16_t index = 0;
	Magma_UINT8toUINT16_LE(&mdb_data.product_price_u16, mdb_com.rx.data_buf, &index);
	Magma_UINT8toUINT16_LE(&mdb_data.product_num_16, mdb_com.rx.data_buf, &index);

	Screen_Sale_UI();
	page = Post_Page;

	//Post
	m95_state = M95_POST_CONFIG;
	end_point.begin_post = 1;

	float tl = mdb_data.product_price_u16 / 100.0f;
	char json_format[200];
	memset(end_point.json_data, 0, strlen((char *)end_point.json_data) + 1);
	vars_to_str((char *)json_format, MGM_NEWSALE_JSON_DATA, mdb_data.product_num_16, tl);
	vars_to_str((char *)end_point.json_data, MGM_NEWSALE_JSON_PARAMETERS, strlen(json_format));
	strcat((char *)end_point.json_data, json_format);

	//mdb_com.begin_session_started = FALSE;
	rfid_begin_session_test_flag = FALSE;
	Send_MDB(Vend_Approved, NULL, 0);
}

void Vend_Cancelled_Handler(void)
{
	//TODO Begin Session yapıldı, 30 saniye içinde seçim yapılmadıysa çağrılacak fonksiyon.

	//TODO State Sıfırla
	mdb_com.tx.data_len_u8 = 0;
	Send_MDB(Vend_Cancelled, mdb_com.tx.data, mdb_com.tx.data_len_u8);
}

void Vend_Denied_Handler(void)
{
	//TODO Bankadan red geldiğinde çağrılacak fonksiyon.

	mdb_com.tx.data_len_u8 = 0;
	Send_MDB(Vend_Denied, mdb_com.tx.data, mdb_com.tx.data_len_u8);
}

void Vend_Approved_Handler(void)
{
	//TODO Başarılı olarak ödeme alındığında çağrılcak fonksiyon.

	mdb_com.tx.data_len_u8 = 0;
	Send_MDB(Vend_Approved, mdb_com.tx.data, mdb_com.tx.data_len_u8);
}

void Vend_Cash_Sale_Handler(void)
{
	//TODO MDB'den Cash Sale komutu geldikçe sunucuya basılacak.
	uint16_t idx = 0;
	mdb_com.tx.data_len_u8 = 0;

	Magma_UINT8toUINT16_LE(&mdb_data.product_num_16, mdb_com.rx.data_buf, &idx);
	Magma_UINT8toUINT16_LE(&mdb_data.product_price_u16, mdb_com.rx.data_buf, &idx);

	Screen_Sale_UI();
	page = Post_Page;

	  if(m95_url != CASHLESS_POST_URL)
	  {
		  strcpy((char*)end_point.url, MAGMA_URL);
		  m95_url = CASHLESS_POST_URL;
		  m95_state = M95_SET_URL_CONFIG;
		  end_point.url_changed = TRUE;
	  }

	  else
	  {
			//Post
			m95_state = M95_POST_CONFIG;
			Convert_Data_2_JSON(mdb_data.product_price_u16, mdb_data.product_num_16, (char *)end_point.json_data);
			end_point.begin_post = 1;
	  }
}

void NFC_Card_Session(uint8_t* card_uid, char *json_data)
{
	char json_data_format[200];
	memset((char *)json_data, 0, strlen((char *)json_data) + 1);
	vars_to_str((char *)json_data_format, MGM_NFC_JSON_CONTENT, card_uid[0], card_uid[1], card_uid[2], card_uid[3], card_uid[4], card_uid[5]);
	vars_to_str((char *)json_data, MGM_NFC_JSON_FORMAT, strlen(json_data_format));
	strcat((char *)json_data, json_data_format);
}

void MDB_RX_Handler(void)
{
	static uint8_t rx_buf[MAX_MDB_DATA_SIZE];
	static uint8_t data_len_u8;
	static uint8_t buffer_idx = 2;
	switch(mdb_com.rx.com_state_u8)
	{
		case Start_Of_Text:
		{
			if(mdb_com.rx.received_byte_u8 == STX)
			{
				mdb_com.rx.com_state_u8 = Command;
			}

			else
			{
				mdb_com.rx.stx_error_ctr_u8++;
			}
			break;
		}

		case Command:
		{
			rx_buf[0] = mdb_com.rx.received_byte_u8;
			mdb_com.rx.com_state_u8 = Data_Length;
			break;
		}

		case Data_Length:
		{
			rx_buf[1] = mdb_com.rx.received_byte_u8;
			data_len_u8 = mdb_com.rx.received_byte_u8;

			if(data_len_u8 != 0)
			{
				mdb_com.rx.com_state_u8 = Data;
			}

			else
			{
				mdb_com.rx.com_state_u8 = Checksum;
			}

			break;
		}

		case Data:
		{
			rx_buf[buffer_idx] = mdb_com.rx.received_byte_u8;

			buffer_idx++;

			if((buffer_idx - 2) == data_len_u8)
			{
				mdb_com.rx.com_state_u8 = Checksum;
			}
			break;
		}

		case Checksum:
		{
			if(Magma_Checksum_Handler(rx_buf, data_len_u8 + 2) == mdb_com.rx.received_byte_u8)
			{
				mdb_com.rx.com_state_u8 = End_Of_Text;
			}

			else
			{
				mdb_com.rx.com_state_u8 = Start_Of_Text;
				memset(rx_buf, 0, MAX_MDB_DATA_SIZE);
				buffer_idx = 3;
				mdb_com.rx.checksum_error_ctr_u8++;
			}
			break;
		}

		case End_Of_Text:
		{
			if(mdb_com.rx.received_byte_u8 == ETX)
			{
				mdb_com.rx.command_u8 = rx_buf[0];
				mdb_com.rx.data_len_u8 = rx_buf[1];

				for(uint8_t i = 0; i < data_len_u8; i++)
				{
					mdb_com.rx.data_buf[i] = rx_buf[i + 2];
				}

				mdb_com.rx_buffer_ready = TRUE;
			}

			else
			{
				mdb_com.rx.etx_error_ctr_u8++;
			}

			memset(rx_buf, 0, MAX_MDB_DATA_SIZE);
			buffer_idx = 2;
			mdb_com.rx.com_state_u8 = Start_Of_Text;
			break;
		}
	}
}
