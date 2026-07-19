//
// Created by Nguyen Hoang Viet on 16/07/2026.
//

#include "lora_receive.h"
#include "peripheral/usart/usart.h"

static volatile uint8_t lora_rx_buffer[LORA_RX_BUFFER_SIZE];
static volatile uint16_t lora_rx_write_index = 0;
static volatile uint16_t lora_rx_read_index = 0;

static uint16_t lora_receive_next_index(uint16_t index);

void LoRa_Receive_Init(void)
{
    LoRa_Receive_Clear_Buffer();
}

uint16_t LoRa_Receive_Available(void)
{
    if (lora_rx_write_index >= lora_rx_read_index) {
        return lora_rx_write_index - lora_rx_read_index;
    }

    return LORA_RX_BUFFER_SIZE - lora_rx_read_index + lora_rx_write_index;
}

LoRa_Receive_Status LoRa_Receive_Read(uint8_t *buffer, uint16_t buffer_size, uint16_t *received_size)
{
    uint16_t count = 0;

    if (buffer == NULL || received_size == NULL || buffer_size == 0) {
        return LORA_RECEIVE_ERROR_ARGUMENT;
    }

    while (LoRa_Receive_Available() > 0 && count < buffer_size) {
        buffer[count] = lora_rx_buffer[lora_rx_read_index];
        lora_rx_read_index = lora_receive_next_index(lora_rx_read_index);
        count++;
    }

    *received_size = count;
    if (count == 0) {
        return LORA_RECEIVE_ERROR_BUFFER_EMPTY;
    }

    return LORA_RECEIVE_OK;
}

void LoRa_Receive_Clear_Buffer(void)
{
    lora_rx_write_index = 0;
    lora_rx_read_index = 0;
}

void LoRa_Receive_Push_Byte(uint8_t received_data)
{
    uint16_t next_write_index = lora_receive_next_index(lora_rx_write_index);

    if (next_write_index == lora_rx_read_index) {
        lora_rx_read_index = lora_receive_next_index(lora_rx_read_index);
    }

    lora_rx_buffer[lora_rx_write_index] = received_data;
    lora_rx_write_index = next_write_index;
}

static uint16_t lora_receive_next_index(uint16_t index)
{
    index++;
    if (index >= LORA_RX_BUFFER_SIZE) {
        index = 0;
    }

    return index;
}
