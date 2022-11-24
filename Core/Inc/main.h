/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

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
void delay_us(uint32_t time);
int debounce(uint32_t time);
void sendString_UART(char*text);
float map(uint16_t val, int in_min, int in_max, int out_min, int out_max);
void displayReadings(int disp_No);
void sendAllReadingsUART();
void analogDeviceReadDMA();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_8
#define B1_GPIO_Port GPIOA
#define B1_EXTI_IRQn EXTI9_5_IRQn
#define T_UP_Pin GPIO_PIN_11
#define T_UP_GPIO_Port GPIOA
#define T_UP_EXTI_IRQn EXTI15_10_IRQn
#define TR_DOWN_Pin GPIO_PIN_12
#define TR_DOWN_GPIO_Port GPIOA
#define TR_DOWN_EXTI_IRQn EXTI15_10_IRQn
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define WATER_PIN_Pin GPIO_PIN_3
#define WATER_PIN_GPIO_Port GPIOB
#define B_MODE_Pin GPIO_PIN_4
#define B_MODE_GPIO_Port GPIOB
#define B_MODE_EXTI_IRQn EXTI4_IRQn
#define B_WATER_Pin GPIO_PIN_5
#define B_WATER_GPIO_Port GPIOB
#define B_WATER_EXTI_IRQn EXTI9_5_IRQn
#define T_DOWN_Pin GPIO_PIN_6
#define T_DOWN_GPIO_Port GPIOB
#define T_DOWN_EXTI_IRQn EXTI9_5_IRQn
#define TR_UP_Pin GPIO_PIN_7
#define TR_UP_GPIO_Port GPIOB
#define TR_UP_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
