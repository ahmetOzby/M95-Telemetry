/*
 * Magma_Endpoint.h
 *
 *  Created on: 29 Eyl 2021
 *      Author: Ahmet
 */

#ifndef INC_MAGMA_ENDPOINT_H_
#define INC_MAGMA_ENDPOINT_H_
#include "Magma_M95.h"

typedef struct
{
	uint8_t begin_post : 1;
	uint8_t discharge_failure_posts : 1;
	uint8_t url_changed : 1;
	uint8_t reserved : 5;
	uint8_t store_failure_idx;
	uint8_t json_data[1024];
	uint8_t url[1024];
	uint16_t stored_parameters[10][2];
}End_Point_T;

End_Point_T end_point;

typedef struct
{
	char 	id[100];
	char 	card_ID[20];
	float 	card_credit_f32;
	uint8_t signal_quality;
	uint8_t card_status				: 1;
	uint8_t reserved				: 7;
}End_Point_Data_t;

End_Point_Data_t end_point_data;

typedef enum
{
	CASHLESS_POST_URL,
	NFC_POST_URL
}url_enum;
url_enum m95_url;


#define MAGMA_URL						"https://mgmtelemetry.herokuapp.com/newSale\0"
#define MAGMA_NFC_URL					"https://apitelemetry.herokuapp.com/payment/cardCredit\0"
#define MAGMA_POST_CASH_SALE			"POST /newSale HTTP/1.1\r\nHost: apitelemetry.herokuapp.com\r\nContent-Type: application/json\r\nContent-Length: %u\r\n\r\n{\"vmcID\": \"02\",\"spiralNumber\":22,\"price\":%f}\0"
#define MGM_NEWSALE_JSON_PARAMETERS		"POST /newSale HTTP/1.1\r\nHost: mgmtelemetry.herokuapp.com\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n\0"
#define MGM_NEWSALE_JSON_DATA			"{\"vmcID\":\"02\",\"spiralNumber\":%d,\"price\":%0.2f}\0"
#define MGM_NFC_JSON_FORMAT				"POST /payment/cardCredit HTTP/1.1\r\nHost: apitelemetry.herokuapp.com\r\nContent-Type: application/json\r\nContent-Length: %u\r\n\r\n\0"
#define MGM_NFC_JSON_CONTENT			"{\"cardID\":\"%u%u%u%u%u%u\"}\0"

#define JSON_KEY_NUM			4
void Post_Handler(char * data);
void End_Point_Response_Handler(void);
void Convert_Data_2_JSON(uint16_t product_price, uint16_t product_num, char *json_data);
void Discharge_Stored_Endpoint_Buf(void);
#endif /* INC_MAGMA_ENDPOINT_H_ */
