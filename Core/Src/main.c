#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "lora_transmit/lora_receive/lora_receive.h"
#include "peripheral/spi/spi.h"
#include "peripheral/gpio/gpio.h"
#include "peripheral/rcc/rcc.h"
#include "peripheral/tim/tim.h"
#include "peripheral/i2c/i2c/i2c.h"
#include "peripheral/i2c/lcd_config/lcd_config.h"
#include "peripheral/usart/usart.h"

#define RX_BUFFER_SIZE 64
char rx_string_buffer[RX_BUFFER_SIZE];
uint8_t rx_index = 0;

RTC_HandleTypeDef hrtc;

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  LoRa_Receive_Clear_Buffer();

  // Kiểm tra khởi tạo cấu hình các ngoại vi cần thiết
  lcd_init();
  lcd_goto_xy(0, 0);
  
  // Kiểm tra đơn giản: nếu MX_USART1_UART_Init chạy qua được mà không crash
  // thì coi như cấu hình phần cứng thành công.
  lcd_send_string("Config: OK      ");
  lcd_goto_xy(1, 0);
  lcd_send_string("Waiting data... ");

  while (1)
  {
    // Fix lỗi so sánh ở đây: sử dụng == thay vì =
    if (usart1_rx_flag == 1) {
      uint8_t received_byte = USART1_Get_Received_Byte();

      // Kiểm tra ký tự kết thúc chuỗi
      if (received_byte == '\n'
        || received_byte == '\r'
        || received_byte == '\0'
        || rx_index >= (RX_BUFFER_SIZE - 1)) {
        if (rx_index > 0) {
          rx_string_buffer[rx_index] = '\0';

          int temp = 0, hum = 0;
          // Parse chuỗi dạng: "Term: XX C, Hum: YY%"
          if (sscanf(rx_string_buffer, "Term: %d C, Hum: %d%%", &temp, &hum) == 2) {
            char lcd_row0[17];
            char lcd_row1[17];
            snprintf(lcd_row0, sizeof(lcd_row0), "Temp: %d C      ", temp);
            snprintf(lcd_row1, sizeof(lcd_row1), "Hum:  %d %%      ", hum);
            
            lcd_goto_xy(0, 0);
            lcd_send_string(lcd_row0);
            lcd_goto_xy(1, 0);
            lcd_send_string(lcd_row1);
          } else {
            // Nếu nhận được chuỗi lỗi hoặc khác dạng (ví dụ "DHT11 Read Err")
            char lcd_row0[17];
            snprintf(lcd_row0, sizeof(lcd_row0), "%-16s", rx_string_buffer);
            lcd_goto_xy(0, 0);
            lcd_send_string(lcd_row0);
            
            // lcd_goto_xy(1, 0);
            // lcd_send_string("Waiting data... ");
          }
          rx_index = 0;
        }
      } else {
        rx_string_buffer[rx_index++] = (char)received_byte;
      }
    }
  }
}

void USART1_RxEventCallback(uint8_t received_data)
{
  // Đẩy byte nhận được từ ngắt vào bộ đệm của LoRa
  LoRa_Receive_Push_Byte(received_data);
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
