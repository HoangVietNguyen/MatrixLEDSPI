//
// Created by Nguyen Hoang Viet on 19/07/2026.
//

#ifndef EMBEDDED_SYSTEM_RTC_H
#define EMBEDDED_SYSTEM_RTC_H

#include "stm32f1xx_hal.h"
#include "stdint.h"

extern RTC_HandleTypeDef hrtc;

// Khởi tạo RTC với LSE 32.768 kHz, prescaler = 32767 → tick = 1 giây
void MX_RTC_Init(void);

// Đưa chip vào STOP mode, RTC Alarm sẽ đánh thức sau `seconds` giây
void RTC_Sleep_Seconds(uint32_t seconds);

// Xóa cờ ngắt RTC Alarm (gọi trong IRQ handler)
void RTC_Clear_Alarm_Flag(void);

#endif //EMBEDDED_SYSTEM_RTC_H
