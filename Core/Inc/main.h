/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Card_Reader_TX_Pin GPIO_PIN_0
#define Card_Reader_TX_GPIO_Port GPIOA
#define Card_Reader_RX_Pin GPIO_PIN_1
#define Card_Reader_RX_GPIO_Port GPIOA
#define MDB_TX_Pin GPIO_PIN_2
#define MDB_TX_GPIO_Port GPIOA
#define MDB_RX_Pin GPIO_PIN_3
#define MDB_RX_GPIO_Port GPIOA
#define SCREEN_RESET_Pin GPIO_PIN_4
#define SCREEN_RESET_GPIO_Port GPIOC
#define SCREEN_DC_Pin GPIO_PIN_5
#define SCREEN_DC_GPIO_Port GPIOC
#define SCREEN_BL_Pin GPIO_PIN_0
#define SCREEN_BL_GPIO_Port GPIOB
#define DEBUG_LED1_Pin GPIO_PIN_12
#define DEBUG_LED1_GPIO_Port GPIOB
#define DEBUG_LED2_Pin GPIO_PIN_13
#define DEBUG_LED2_GPIO_Port GPIOB
#define DEBUG_LED3_Pin GPIO_PIN_14
#define DEBUG_LED3_GPIO_Port GPIOB
#define VBAT_ON_OFF_Pin GPIO_PIN_8
#define VBAT_ON_OFF_GPIO_Port GPIOA
#define M95_TX_Pin GPIO_PIN_9
#define M95_TX_GPIO_Port GPIOA
#define PWR_KEY_Pin GPIO_PIN_7
#define PWR_KEY_GPIO_Port GPIOD
#define SCREEN_CS_Pin GPIO_PIN_5
#define SCREEN_CS_GPIO_Port GPIOB
#define M95_RX_Pin GPIO_PIN_7
#define M95_RX_GPIO_Port GPIOB
#define M95_EMERG_OFF_Pin GPIO_PIN_1
#define M95_EMERG_OFF_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
