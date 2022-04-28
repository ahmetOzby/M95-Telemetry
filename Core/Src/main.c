/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Magma_Endpoint.h"
#include "Magma_GL.h"
#include "Magma_M95.h"
#include "Magma_Protocol_Stack.h"
#include "Magma_Typecast.h"
#include "ST7735.h"
#include "Fonts.h"
#include "Images.h"
#include "Magma_Screen.h"
#include "Proxy_Mifare.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rfid_begin_session_test_flag;
uint8_t x, y;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&M95_UART, &m95_com.received_byte_u8, 1);
  page = Intro_Page;
  Screen_Opening();
  Screen_Idle_Page(1, 0);
  //M95_Init();
  HAL_UART_Receive_IT(&MDB_UART, &mdb_com.rx.received_byte_u8, 1);
  HAL_UART_Receive_IT(&NFC_UART, &reader_com.received_byte_u8, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(sys_timer._100_hz == TRUE)
	  {
		  //POST PROCCESS BEGIN
		  if(end_point.begin_post == TRUE || reader_data.nfc_session_started == TRUE)
		  {
			  Post_Handler((char *)end_point.json_data);		//Endpointten gelen cevaba göre bufferlar doldurulur.
		  }

		  if(end_point.discharge_failure_posts == TRUE)
		  {
			  //Discharge_Stored_Endpoint_Buf();
		  }

		  //Kacirilan Post 4u gecerse reset.
		  /*if((m95_errors.post_timeout_ctr_u32 + m95_errors.get_timeout_ctr_u32) > 4)
		  {
			  m95_errors.get_timeout_ctr_u32 = 0;
			  m95_errors.post_timeout_ctr_u32 = 0;
			  m95_state = M95_COM_CONTROL;
			  ST7735_FillScreen(ST7735_BLACK);
			  page = Intro_Page;
			  M95_Init();
		  }*/

		  /*switch(m95_state)
		  {
		  	  case M95_SIGNAL_QUALITY_CHECK:
		  	  {
		  		  if(page != Idle_Page)
				  {
					  Screen_Idle_Page(1, m95_com.internet_con_ok);
					  screen_data.internet_con_changed = 0;
				  }

		  		  static uint8_t function_called = 0;
		  		  if (function_called == 0)
		  		  {
		  			  memset(m95_com.rx_buf, 0, 1024);
		  			  M95_Get_Signal_Quality(100);
		  			  function_called = 1;
		  			  m95_data._2_sec_timeout = START;
		  			  m95_data._2_sec_ctr_u16 = 0;
		  		  }

		  		  if(m95_com.pack_received == TRUE)
		  		  {
		  			  uint8_t number_start_idx, end_idx;

		  			  function_called = 0;
		  			  number_start_idx = Find_String_Index("+CSQ: ", (char *)m95_com.rx_buf);
		  			  end_idx = Find_String_Index(",", (char *)m95_com.rx_buf);

					  if(number_start_idx != 0)
					  {
						  char signal_quality[2];

						  for(uint8_t i = 0; i < (end_idx - number_start_idx); i++)
						  {
							  signal_quality[i] = m95_com.rx_buf[number_start_idx + i];
						  }

						  end_point_data.signal_quality = atoi(signal_quality);
					  }
		  		  }

		  		  if(m95_data._2_sec_timeout == TIME_UP)
		  		  {

		  		  }
					  //End Point com failure buffer
		  		  break;
		  	}

		  	case M95_SET_URL_CONFIG:
		  	{
		  		static uint8_t function_called = 0;
		  		m95_data._270_ms_ctr_u16 = 0;
		  		m95_data._270_ms_timeout = START;
		  		if(function_called == 0)
		  		{
		  			m95_com.connect_received = FALSE;
		  			M95_Set_URL_Config(100, strlen((char *)end_point.url), 30);
		  			function_called = 1;
		  		}

				if(m95_com.connect_received == TRUE)
				{
					m95_state = M95_SET_URL;
					m95_com.connect_received = FALSE;
					m95_com.end_point_con_ok = TRUE;
					m95_com.cme_error_rxed = FALSE;
					function_called = 0;
				}

				else if(m95_com.error_received == TRUE)
				{
					Find_String_Index("+CME ERROR:", (char *)m95_com.rx_buf);
					m95_com.cme_error_rxed = TRUE;
					m95_com.end_point_con_ok = FALSE;
					m95_state = M95_SIGNAL_QUALITY_CHECK;
				}

		  		else if(m95_data._270_ms_timeout == TIME_UP)
		  		{
		  			//TODO SET URL CONFIG TIMEOUT
		  			m95_state = M95_SIGNAL_QUALITY_CHECK;
		  		}
		  		break;
		  	}

		  	case M95_SET_URL:
		  	{
		  		static uint8_t function_called = 0;

		  		if(function_called == 0)
		  		{
		  			m95_com.ok_received = FALSE;
		  			m95_data._270_ms_ctr_u16 = 0;
		  			m95_data._270_ms_timeout = START;
		  			M95_Set_URL(M95_UART_TIMEOUT, end_point.url);
		  			function_called = 1;
		  		}

				if(m95_com.ok_received == TRUE)
				{
					function_called = 0;
					m95_com.ok_received = FALSE;
					end_point.url_changed = FALSE;

					if(m95_url == CASHLESS_POST_URL)
					{
						m95_state = M95_POST_CONFIG;
						Convert_Data_2_JSON(mdb_data.product_price_u16, mdb_data.product_num_16, (char *)end_point.json_data);
						end_point.begin_post = 1;
					}
				}

				else if(m95_data._270_ms_timeout == TIME_UP)
				{
					//TODO SET URL TIMEOUT
					m95_state = M95_SIGNAL_QUALITY_CHECK;
				}
		  		break;
		  	}
		  	default: break;
		  }*/

		  //RFID kart okutuldugundaki case
			if(reader_data.nfc_card_read_flag == 1 /*&& end_point.begin_post != TRUE*/ && reader_data.nfc_session_started != TRUE)
			{
			  /*uint8_t example_uid[6] = {1, 2, 3, 1, 2, 3};

			  if(m95_url != NFC_POST_URL)
			  {
				  strcpy((char*)end_point.url, MAGMA_NFC_URL);
				  m95_url = NFC_POST_URL;
				  m95_state = M95_SET_URL_CONFIG;
				  end_point.url_changed = TRUE;
			  }

			  if(end_point.url_changed == FALSE)
			  {
				  NFC_Card_Session(example_uid, (char*)end_point.json_data);
				  reader_data.nfc_session_started = TRUE;
				  reader_data.nfc_card_read_flag = FALSE;
				  m95_state = M95_POST_CONFIG;
			  }*/
				if(mdb_com.begin_session_started != TRUE)
				{
					Begin_Session_Handler(0, 200);
					mdb_data._30sec_cancel_ctr_u16 = START;
					mdb_com.begin_session_started = TRUE;
				}
			}

			 sys_timer._100_hz = FALSE;
	  }

	  //MDB Command Handler
	  if(sys_timer._10_hz == TRUE)
	  {
		  static uint16_t mdb_response_ctr_u16 = 0;
		  mdb_response_ctr_u16 ++;

		  if(mdb_com.rx_buffer_ready == TRUE)
		  {
			  switch(mdb_com.rx.command_u8)
			  {
				  case Vend_Poll:
				  {
					  //Poll komutunun cevabında ilk byte olarak otomat durumu alınır.
					  switch((uint8_t)mdb_com.rx.data_buf[0])
					  {
						  case Vend_Approved:
						  {
							  Vend_Approved_Handler();
							  mdb_com.begin_session_started = FALSE;
							  break;
						  }
					  }
					  t_vend_state = mdb_com.rx.data_buf[0];

					  switch((uint8_t)t_vend_state)
					  {
						  case VendState_VendRequested:
						  {
							  Send_MDB(Vend_Request, NULL, 0);
							  break;
						  }

						  default: break;
					  }
					  break;
				  }

				  case Cash_Sale:
				  {
					  Vend_Cash_Sale_Handler();
					  break;
				  }

				  case Vend_Request:
				  {
					  Vend_Request_Handler();
					  break;
				  }
			  }

			  mdb_response_ctr_u16 = 0;
			  mdb_data.error_message_u8 &= ~MDB_Com_Error;


			  mdb_com.rx_buffer_ready = FALSE;
		  }

		  if(mdb_com.begin_session_started == TRUE && mdb_data._30sec_cancel_timeout == TIME_UP)
		  {
			  //TODO: VEND CANCEL
			  Send_MDB(Vend_Denied, NULL, 0);
		  }

		  else if(mdb_response_ctr_u16 >= MDB_MAX_RESPONSE_TIME * 2)
		  {
			  mdb_data.error_message_u8 |= MDB_Com_Error;
			  mdb_response_ctr_u16 = MDB_MAX_RESPONSE_TIME * 2;
		  }

		  sys_timer._10_hz = FALSE;
	  }

	  if(sys_timer._5_hz == TRUE)
	  {
		  Send_MDB(Vend_Poll, NULL, 0);

		  sys_timer._5_hz = FALSE;
	  }


	  //Bu kisim m95 fonksiyonlarinin zamanlamasi icin flag ayarlamalarini icerir.
	  if(sys_timer._1_hz == TRUE)
	  {
		  m95_data._12_sec_ctr_u16++;
		  m95_data._16_sec_ctr_u16++;
		  m95_data._10_sec_ctr_u16++;
		  m95_data._20_sec_ctr_u16++;
		  m95_data._3_sec_ctr_u16++;
		  m95_data._2_sec_ctr_u16++;
		  mdb_data._30sec_ctr_u16++;

		  mdb_data._30sec_cancel_ctr_u16++;

		  if(mdb_data._30sec_cancel_ctr_u16 % 30 == 0)
		  {
			  mdb_data._30sec_cancel_timeout = TIME_UP;
		  }
		  if(m95_state == M95_SIGNAL_QUALITY_CHECK && m95_com.internet_con_ok == TRUE)
		  {
			  //Send_MDB(Cash_Sale, mdb_com.tx.data, Cash_Sale_Data_Size);
		  }

		  if(m95_data._2_sec_ctr_u16 %  2 == 0)
		  {
			  m95_data._2_sec_timeout = TIME_UP;
		  }

		  if(m95_data._3_sec_ctr_u16 %  3 == 0)
		  {
			  m95_data._3_sec_timeout = TIME_UP;
		  }

		  if(m95_data._10_sec_ctr_u16 %  10 == 0)
		  {
			  m95_data._10_sec_timeout = TIME_UP;
		  }

		  if(m95_data._12_sec_ctr_u16 %  12 == 0)
		  {
			  m95_data._12_sec_timeout = TIME_UP;
		  }

		  if(m95_data._16_sec_ctr_u16 % 16 == 0)
		  {
			  m95_data._16_sec_timeout = TIME_UP;
		  }

		  if(m95_data._20_sec_ctr_u16 % 20 == 0)
		  {
			  m95_data._20_sec_timeout = TIME_UP;
		  }

		  if(mdb_data._30sec_ctr_u16 % 30 == 0)
		  {
			  mdb_data._30sec_timeout = TIME_UP;
		  }
		  sys_timer._1_hz = FALSE;
	  }

	  if(sys_timer._0_1_hz == TRUE)
	  {
		  if(m95_com.internet_con_ok == TRUE && end_point.store_failure_idx > 0 && m95_state == M95_SIGNAL_QUALITY_CHECK)
		  {
			  end_point.discharge_failure_posts = 1;
		  }

		  //10 Saniyede bir google'a get istegi atilarak internet baglantisi kontrolu
		  if(m95_state == M95_SIGNAL_QUALITY_CHECK)
		  {
			  //m95_state = M95_GOOGLE_PING;
		  }
		  sys_timer._0_1_hz = FALSE;
	  }


	  if(sys_timer._1000_hz == TRUE)
	  {
		  m95_data._100_ms_ctr_u16++;
		  m95_data._270_ms_ctr_u16++;

		  if(m95_data._100_ms_ctr_u16 % 100 == 0)
		  {
			  m95_data._100_ms_timeout = TIME_UP;
		  }

		  if(m95_data._270_ms_ctr_u16 % 270 == 0)
		  {
			  m95_data._270_ms_timeout = TIME_UP;
		  }
		  sys_timer._1000_hz = FALSE;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SCREEN_RESET_Pin|SCREEN_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCREEN_BL_Pin|DEBUG_LED1_Pin|DEBUG_LED2_Pin|DEBUG_LED3_Pin
                          |SCREEN_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(VBAT_ON_OFF_GPIO_Port, VBAT_ON_OFF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_KEY_GPIO_Port, PWR_KEY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(M95_EMERG_OFF_GPIO_Port, M95_EMERG_OFF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SCREEN_RESET_Pin SCREEN_DC_Pin */
  GPIO_InitStruct.Pin = SCREEN_RESET_Pin|SCREEN_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SCREEN_BL_Pin DEBUG_LED1_Pin DEBUG_LED2_Pin DEBUG_LED3_Pin
                           SCREEN_CS_Pin */
  GPIO_InitStruct.Pin = SCREEN_BL_Pin|DEBUG_LED1_Pin|DEBUG_LED2_Pin|DEBUG_LED3_Pin
                          |SCREEN_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : VBAT_ON_OFF_Pin */
  GPIO_InitStruct.Pin = VBAT_ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(VBAT_ON_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_KEY_Pin */
  GPIO_InitStruct.Pin = PWR_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : M95_EMERG_OFF_Pin */
  GPIO_InitStruct.Pin = M95_EMERG_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(M95_EMERG_OFF_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &M95_UART)
	{
		M95_RX_Callback_Handler();
		HAL_UART_Receive_IT(&M95_UART, &m95_com.received_byte_u8, 1);
	}

	else if(huart == &MDB_UART)
	{
		MDB_RX_Handler();
		HAL_UART_Receive_IT(&MDB_UART, &mdb_com.rx.received_byte_u8, 1);
	}

	else if(huart == &NFC_UART)
	{
		Reader_RX_Handler();
		HAL_UART_Receive_IT(&NFC_UART, &reader_com.received_byte_u8, 1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
