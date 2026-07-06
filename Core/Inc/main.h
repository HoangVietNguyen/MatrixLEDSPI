/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_TEST_Pin GPIO_PIN_4
#define LED_TEST_GPIO_Port GPIOA
#define BUTTON_SHIFT_LEFT_Pin GPIO_PIN_11
#define BUTTON_SHIFT_LEFT_GPIO_Port GPIOA
#define BUTTON_SHIFT_LEFT_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_SHIFT_RIGHT_Pin GPIO_PIN_12
#define BUTTON_SHIFT_RIGHT_GPIO_Port GPIOA
#define BUTTON_SHIFT_RIGHT_EXTI_IRQn EXTI15_10_IRQn

#define REG_DIGIT_1      0x01
#define REG_DIGIT_2      0x02
#define REG_DIGIT_3      0x03
#define REG_DIGIT_4      0x04
#define REG_DIGIT_5      0x05
#define REG_DIGIT_6      0x06
#define REG_DIGIT_7      0x07
#define REG_DIGIT_8      0x08
#define REG_DECODE_MODE  0x09
#define REG_INTENSITY    0x0A
#define REG_SCAN_LIMIT   0x0B
#define REG_SHUTDOWN     0x0C
#define REG_DISPLAY_TEST 0x0F

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
