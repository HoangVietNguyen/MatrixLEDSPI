//
// Created by Nguyen Hoang Viet on 19/07/2026.
//

#include "lora_config.h"
#include "peripheral/usart/usart.h"
#include <string.h>

#define LORA_CONFIG_TIMEOUT_MS  1000U

LoRa_Config_Status LoRa_Configure_Module(uint16_t address, uint8_t channel)
{
    uint8_t config_cmd[6];
    uint8_t config_resp[6] = {0};

    // 1. Chuẩn bị tập lệnh cấu hình 6-byte cho E32
    config_cmd[0] = 0xC0;               // Mở đầu: Cấu hình lưu trữ khi mất điện
    config_cmd[1] = (address >> 8) & 0xFF; // ADDH (Địa chỉ cao)
    config_cmd[2] = address & 0xFF;        // ADDL (Địa chỉ thấp)
    config_cmd[3] = 0x1A;               // SPED: 8N1, 9600bps, Air data rate 2.4kbps
    config_cmd[4] = channel;            // CHAN: Kênh tần số (tần số = 410M + CHAN * 1M)
    config_cmd[5] = 0x44;               // OPTION: Chế độ truyền Transparent (0x44), công suất tối đa

    // Tạm thời dừng ngắt nhận UART trên Receiver để tránh ngắt nhận đè lên dữ liệu cấu hình
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    // 2. Gửi 6 byte cấu hình sang LoRa qua USART1
    if (USART1_Send_Data(config_cmd, sizeof(config_cmd), LORA_CONFIG_TIMEOUT_MS) != HAL_OK)
    {
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        return LORA_CONFIG_ERROR_TIMEOUT;
    }

    // 3. Chờ LoRa phản hồi lại 6 byte cấu hình (đọc dạng blocking)
    if (USART1_Receive_Data(config_resp, sizeof(config_resp), LORA_CONFIG_TIMEOUT_MS) != HAL_OK)
    {
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        return LORA_CONFIG_ERROR_TIMEOUT;
    }

    // Kích hoạt lại ngắt nhận UART cho hoạt động truyền nhận dữ liệu sau đó
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    USART1_Start_Receive_IT();

    // 4. Kiểm tra phản hồi có khớp với lệnh gửi đi không
    if (memcmp(config_cmd, config_resp, sizeof(config_cmd)) != 0)
    {
        return LORA_CONFIG_ERROR_RESPONSE;
    }

    return LORA_CONFIG_OK;
}
