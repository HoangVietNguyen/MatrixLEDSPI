//
// Created by Nguyen Hoang Viet on 06/07/2026.
//

#ifndef MATRIXLEDSPI_LCD_I2C_H
#define MATRIXLEDSPI_LCD_I2C_H

#define LCD_ADDR (0x27 << 1)

void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_init(void);
void lcd_goto_xy(uint8_t row, uint8_t col);
void lcd_send_string(char *str);
void MX_I2C1_Init(void);
#endif //MATRIXLEDSPI_LCD_I2C_H