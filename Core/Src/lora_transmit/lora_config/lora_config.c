//
// Created by Nguyen Hoang Viet on 19/07/2026.
//

#include "lora_config.h"
#include "peripheral/usart/usart.h"
#include "main.h"  // Chứa macro LORA_M0_Pin, LORA_M1_Pin, LORA_AUX_Pin, v.v.
#include <string.h>

#define LORA_CONFIG_TIMEOUT_MS  1000U

// Hàm đợi mô-đun LoRa xử lý xong (AUX = 1 là rảnh)
static uint8_t LoRa_Wait_Busy(void)
{
    uint32_t timeout_tick = HAL_GetTick();

    // Đợi khi chân AUX còn ở mức LOW (LoRa đang bận)
    while (HAL_GPIO_ReadPin(LORA_AUX_GPIO_Port, LORA_AUX_Pin) == GPIO_PIN_RESET)
    {
        // Nếu quá 1000ms (1 giây) mà AUX không lên High -> báo lỗi và thoát
        if ((HAL_GetTick() - timeout_tick) > 1000U)
        {
            return 0; // Thất bại (Timeout)
        }
        HAL_Delay(1);
    }
    HAL_Delay(2);
    return 1; // Thành công
}

// Hàm đổi chế độ hoạt động của LoRa bằng cách kéo chân M0, M1
static void LoRa_Set_Mode(GPIO_PinState m0_state, GPIO_PinState m1_state)
{
    HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, m0_state);
    HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, m1_state);
    
    // Đợi module chuyển đổi trạng thái xong
    LoRa_Wait_Busy();
}

LoRa_Config_Status LoRa_Configure_Module(uint16_t address, uint8_t channel)
{
    uint8_t config_cmd[6];
    uint8_t config_resp[6] = {0};

    // === BƯỚC QUAN TRỌNG: Hủy ngắt nhận IT đang chạy để đưa huart về READY ===
    // Nếu không hủy, HAL_UART_Receive() sẽ trả về HAL_BUSY và không đọc được phản hồi cấu hình.
    HAL_UART_AbortReceive(&huart1);

    // 1. Chuyển LoRa sang chế độ cấu hình (M0 = 1, M1 = 1)
    LoRa_Set_Mode(GPIO_PIN_SET, GPIO_PIN_SET);

    // 2. Chuẩn bị tập lệnh cấu hình 6-byte cho E32
    config_cmd[0] = 0xC0;               // Mở đầu: Cấu hình lưu trữ khi mất điện
    config_cmd[1] = (address >> 8) & 0xFF; // ADDH (Địa chỉ cao)
    config_cmd[2] = address & 0xFF;        // ADDL (Địa chỉ thấp)
    config_cmd[3] = 0x1A;               // SPED: 8N1, 9600bps, Air data rate 2.4kbps
    config_cmd[4] = channel;            // CHAN: Kênh tần số (tần số = 410M + CHAN * 1M)
    config_cmd[5] = 0x44;               // OPTION: Chế độ truyền Transparent (0x44), công suất tối đa

    // 3. Gửi 6 byte cấu hình sang LoRa qua USART1 (dạng blocking)
    if (USART1_Send_Data(config_cmd, sizeof(config_cmd), LORA_CONFIG_TIMEOUT_MS) != HAL_OK)
    {
        // Trả LoRa về chế độ Normal và kích hoạt lại ngắt nhận UART
        LoRa_Set_Mode(GPIO_PIN_RESET, GPIO_PIN_RESET);
        USART1_Start_Receive_IT();
        return LORA_CONFIG_ERROR_TIMEOUT;
    }

    // 4. Chờ LoRa phản hồi lại 6 byte cấu hình (dạng blocking)
    if (USART1_Receive_Data(config_resp, sizeof(config_resp), LORA_CONFIG_TIMEOUT_MS) != HAL_OK)
    {
        LoRa_Set_Mode(GPIO_PIN_RESET, GPIO_PIN_RESET);
        USART1_Start_Receive_IT();
        return LORA_CONFIG_ERROR_TIMEOUT;
    }

    // 5. Cấu hình thành công, đưa LoRa về chế độ chạy bình thường (M0 = 0, M1 = 0)
    LoRa_Set_Mode(GPIO_PIN_RESET, GPIO_PIN_RESET);

    // 6. Kích hoạt lại chế độ nhận ngắt IT chuẩn bị nhận dữ liệu không dây
    USART1_Start_Receive_IT();

    // 7. Kiểm tra phản hồi có khớp với lệnh gửi đi không
    if (memcmp(config_cmd, config_resp, sizeof(config_cmd)) != 0)
    {
        return LORA_CONFIG_ERROR_RESPONSE;
    }

    return LORA_CONFIG_OK;
}
