/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include "ICMax7219_driver/ICMax7219_driver.h"
#include "spi_driver/spi_driver.h"
#include "gpio_driver/gpio_driver.h"
#include "rcc_driver/rcc_driver.h"
#include "tim_driver/tim_driver.h"
#include "generate_piece_driver/pieces.h"
#include "lcd_i2c_driver/lcd_i2c.h"
#include "dht11_driver/dht11.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

RTC_HandleTypeDef hrtc;
volatile uint16_t hight_score  = 100;
volatile uint8_t rtc_alarm_wakeup_flag = 0;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

void MX_SPI1_Init(void);
void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

extern SPI_HandleTypeDef hspi1; // gọi cấu hình spi trong spi_driver.c

// hàm click button, khối hình dịch sang trái hoặc phảo
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN) {
  if (GPIO_PIN == BUTTON_SHIFT_LEFT_Pin) {
    MAX7219_Shift_Left_Pieces();
  }

  if (GPIO_PIN == BUTTON_SHIFT_RIGHT_Pin) {
    MAX7219_Shift_Right_Pieces();
  }

}

//hàm interrupt bắt sự kiện đếm hết 500ms, tạo ra một hình khối
 void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
 {
   if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
   {
     MAX7219_Generate_Pieces();
   }
 }

//interrupt mỗi 500ms, dịch khối hình xuống một nấc
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim ->Instance == TIM1) {
    MAX7219_Shift_Down_Pieces();
  }
}

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
  MX_TIM1_Init();
  MX_I2C1_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
  /* USER CODE BEGIN 2 */
  lcd_init();
  char high_score_data[16];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* USER CODE BEGIN 2 */
  MAX7219_Init_System();       // Khởi tạo phần cứng IC
  /* USER CODE END 2 */
  while (1)
  {
    /* USER CODE END WHILE */
    sprintf(high_score_data, "High score: %04d", hight_score);
    lcd_goto_xy(0, 0);
    lcd_send_string(high_score_data);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

// SystemClock_Config is now in rcc_driver/rcc_driver.c

// MX_TIM1_Init is now in tim_driver/tim_driver.c

// MX_GPIO_Init is now implemented in gpio_driver/gpio_driver.c



/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
