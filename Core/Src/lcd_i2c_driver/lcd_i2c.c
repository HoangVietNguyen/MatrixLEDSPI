//
// Created by Nguyen Hoang Viet on 06/07/2026.
//

#include <stdio.h>
#include "stdint.h"
#include "lcd_i2c.h"

#include "main.h"
#include "stm32f1xx_hal_i2c.h"

I2C_HandleTypeDef hi2c1;

void lcd_send_cmd(char cmd) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xF0 );
    data_l = ((cmd << 4) & 0xF0);
    data_t[0] = data_u | 0x0C;  // RS = 0, EN = 1
    data_t[1] = data_u | 0x08;  // RS = 0, EN = 0
    data_t[2] = data_l | 0x0C;  // RS = 0, EN = 1
    data_t[3] = data_l | 0x08;  // RS = 0, EN = 0
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_send_data(char data) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xF0 );
    data_l = ((data << 4) & 0xF0);
    data_t[0] = data_u | 0x0D;  // RS = 1, EN = 1  (Vì 0x0C | 0x01 = 0x0D)
    data_t[1] = data_u | 0x09;  // RS = 1, EN = 0  (Vì 0x08 | 0x01 = 0x09)
    data_t[2] = data_l | 0x0D;  // RS = 1, EN = 1
    data_t[3] = data_l | 0x09;  // RS = 1, EN = 0
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_init(void) {
    HAL_Delay(50);
    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x32);
    HAL_Delay(10);

    lcd_send_cmd(0x28);
    HAL_Delay(1);
    lcd_send_cmd(0x0C);
    HAL_Delay(1);
    lcd_send_cmd(0x01);
    HAL_Delay(2);
    lcd_send_cmd(0x06);
}

void lcd_goto_xy(uint8_t row, uint8_t col) {
    uint8_t address;
    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }
    lcd_send_cmd(address);
}

void lcd_send_string(char *str) {
    while (*str) lcd_send_data(*str++);
}

void MX_I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */

}
