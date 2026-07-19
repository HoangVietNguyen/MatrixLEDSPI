#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "peripheral/spi/spi.h"
#include "peripheral/gpio/gpio.h"
#include "peripheral/rcc/rcc.h"
#include "peripheral/tim/tim.h"
#include "peripheral/i2c/i2c/i2c.h"
#include "peripheral/rtc/rtc.h"
#include "peripheral/usart/usart.h"
#include "sensor_manager/dht11_driver/dht11.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_12

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_RTC_Init();       // Khởi tạo RTC trước khi dùng RTC_Sleep_Seconds
  MX_USART1_UART_Init();

  HAL_DBGMCU_EnableDBGStopMode(); // Debugmodul erhalten als Processor geschlaft hat

  DHT11_Init(DHT11_PORT, DHT11_PIN);

  while (1)
  {
    DHT11_Data data;
    char payload[32];

    if (DHT11_Read(&data) == DHT11_OK) {
      snprintf(payload, sizeof(payload), "Term: %d C, Hum: %d%%", data.temperature, data.humidity);
    } else {
      snprintf(payload, sizeof(payload), "DHT11 Read Err");
    }

    USART1_Send_String(payload);
    HAL_GPIO_WritePin(LED_TEST_GPIO_Port, LED_TEST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED_TEST_GPIO_Port, LED_TEST_Pin, GPIO_PIN_RESET);

    RTC_Sleep_Seconds(2);
  }

}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
#endif
