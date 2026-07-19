//
// Created by Nguyen Hoang Viet on 19/07/2026.
//

#include "rtc.h"
#include "main.h"
#include "peripheral/rcc/rcc.h"

RTC_HandleTypeDef hrtc;

// ============================================================
// MX_RTC_Init
// Khởi tạo RTC dùng LSI (RC nội ~40kHz) để tránh lỗi không có LSE.
// ============================================================
void MX_RTC_Init(void)
{
    hrtc.Instance          = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND; // Tự tính toán prescaler cho 1 Hz từ nguồn clock LSI
    hrtc.Init.OutPut       = RTC_OUTPUTSOURCE_NONE;

    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }

    // Bật ngắt RTC Alarm trong NVIC
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

// ============================================================
// HAL_RTC_MspInit
// Cấu hình clock nguồn cho RTC (LSI).
// Được gọi tự động bên trong HAL_RTC_Init().
// ============================================================
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc_param)
{
    if (hrtc_param->Instance == RTC)
    {
        // Bật clock PWR và BKP để truy cập backup domain
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_RCC_BKP_CLK_ENABLE();

        // Cho phép ghi vào backup domain
        HAL_PWR_EnableBkUpAccess();

        // Dùng LSI (RC nội ~40kHz) thay LSE để đảm bảo hoạt động trên mọi board Bluepill
        __HAL_RCC_LSI_ENABLE();
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET); // Chờ LSI ổn định (~1ms)

        __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);
        __HAL_RCC_RTC_ENABLE();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc_param)
{
    if (hrtc_param->Instance == RTC)
    {
        __HAL_RCC_RTC_DISABLE();
        HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
    }
}

// ============================================================
// RTC_Sleep_Seconds
// Đặt RTC Alarm sau `seconds` giây, sau đó đưa chip vào STOP mode.
// Chip sẽ thức dậy khi Alarm kích hoạt ngắt RTC_Alarm_IRQHandler.
// Sau khi thức dậy, SystemClock_Config() được gọi lại để khôi phục clock.
//
// LƯU Ý QUAN TRỌNG: Trên STM32F1, RTC Alarm đi qua EXTI Line 17.
// Phải bật EXTI17 trước khi vào STOP mode, nếu không chip sẽ ngủ mãi.
// ============================================================
void RTC_Sleep_Seconds(uint32_t seconds)
{
    // Đọc giá trị counter hiện tại
    uint32_t counter = ((uint32_t)RTC->CNTH << 16) | RTC->CNTL;
    uint32_t alarm   = counter + seconds;

    // Ghi giá trị Alarm vào thanh ghi RTC
    HAL_PWR_EnableBkUpAccess();
    while (!(RTC->CRL & RTC_CRL_RTOFF));    // Chờ RTC không bận
    RTC->CRL |=  RTC_CRL_CNF;              // Vào chế độ cấu hình
    RTC->ALRH = (alarm >> 16) & 0xFFFF;
    RTC->ALRL =  alarm        & 0xFFFF;
    RTC->CRL &= ~RTC_CRL_CNF;              // Thoát chế độ cấu hình
    while (!(RTC->CRL & RTC_CRL_RTOFF));    // Chờ ghi xong

    // Xóa cờ Alarm cũ và bật ngắt RTC Alarm
    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
    __HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_ALRA);

    // === BẮT BUỘC: Bật EXTI Line 17 để RTC Alarm đánh thức chip khỏi STOP mode ===
    __HAL_RTC_ALARM_EXTI_ENABLE_IT();
    __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();
    __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    // Đưa chip vào STOP mode — CPU dừng, chỉ LSE và RTC tiếp tục chạy
    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    HAL_ResumeTick();

    // === Chip thức dậy tại đây ===
    // Bắt buộc cấu hình lại clock vì STOP mode tắt HSE/PLL
    SystemClock_Config();
}

// ============================================================
// RTC_Clear_Alarm_Flag
// Gọi trong RTC_Alarm_IRQHandler để xóa cờ ngắt RTC và EXTI17.
// ============================================================
void RTC_Clear_Alarm_Flag(void)
{
    // Xóa cờ RTC Alarm
    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
    // Xóa cờ EXTI Line 17
    __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}
