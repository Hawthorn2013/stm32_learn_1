#include "bsp.h"

volatile uint8_t USART1_RX_data;
volatile uint8_t USART2_RX_data;

void Delay_us(uint32_t us)
{
    volatile uint16_t us_i = us;
    while(us_i--)
        ;
}

void Send_To_Console(const char *data, uint32_t len)
{
    uint32_t i = 0;
    for (i = 0; i < len; i++)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)(data + i), 1, 10);
    }
}

void OLED_Reset()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
    Delay_us(1000);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
    Delay_us(1000);
}

void OLED_SendCmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);       //指定发送cmd
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);       //建立片选
    HAL_SPI_Transmit(&hspi2, &cmd, 8, 10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);         //清除片选
}

void OLED_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);         //指定发送data
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 8, 10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        HAL_UART_Transmit(huart, (uint8_t *)&USART1_RX_data, 1, 10);
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&USART1_RX_data, 1);
    }
    else if (huart == &huart2)
    {
        HAL_UART_Transmit(huart, (uint8_t *)&USART2_RX_data, 1, 10);
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&USART2_RX_data, 1);
    }
}
