#include "bsp.h"

volatile uint8_t USART1_RX_data;
volatile uint8_t USART2_RX_data;
BSP_FIFO BSP_FIFO_USART2;

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
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);         //清除片选
}

void OLED_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);         //指定发送data
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 1, 10);
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
        BSP_PushFIFOByte(&BSP_FIFO_USART2, USART2_RX_data);
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&USART2_RX_data, 1);
    }
}

int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
    return HAL_UART_Transmit(&huart2, buf, buflen, 10);
}

int transport_getdata(unsigned char* buf, int count)
{
    while (BSP_GetFIFOAvailableLen(&BSP_FIFO_USART2) <count)
        ;
    BSP_PopFIFO(&BSP_FIFO_USART2, buf, count);
    return count;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
    if (BSP_GetFIFOAvailableLen(&BSP_FIFO_USART2) <count)
    {
        return 0;
    }
    BSP_PopFIFO(&BSP_FIFO_USART2, buf, count);
    return count;
}

int transport_open(char* host, int port)
{
    //使用串口TCP透传，打开不受控制。
    return 0;
}

int transport_close(int sock)
{
    //使用串口TCP透传，关闭不受控制。
    return 0;
}

void BSP_InitFIFO(BSP_FIFO *fifo)
{
    fifo->head = 0;
    fifo->tail = 0;
}

uint32_t BSP_GetFIFOAvailableLen(BSP_FIFO *fifo)
{
    if (fifo->head >= fifo->tail)
    {
        return fifo->head - fifo->tail;
    }
    else
    {
        return (fifo->head + BSP_USART_FIFO_LEN) - fifo->tail;
    }
}

uint32_t BSP_PopFIFOByte(BSP_FIFO *fifo, uint8_t *data)
{
    if (BSP_GetFIFOAvailableLen(fifo))
    {
        *data = (fifo->data)[fifo->tail];
        (fifo->tail)++;
        fifo->tail %= BSP_USART_FIFO_LEN;
        return 1;
    }
    return 0;
}

uint32_t BSP_PopFIFO(BSP_FIFO *fifo, uint8_t *buff, uint32_t len)
{
    uint32_t i;
    
    for (i = 0; i < len; i++)
    {
        if (!BSP_PopFIFOByte(fifo, buff + i))
        {
            break;
        }
    }
    return i;
}

void BSP_PushFIFOByte(BSP_FIFO *fifo, uint8_t data)
{
    (fifo->data)[fifo->head] = data;
    fifo->head++;
    fifo->head %= BSP_USART_FIFO_LEN;
    //溢出发生时覆盖最老的数据
    if (fifo->head == fifo->tail)
    {
        fifo->tail++;
    }
}

void BSP_PushFIFO(BSP_FIFO *fifo, uint8_t *buff, uint32_t len)
{
    uint32_t i;
    
    for (i = 0; i < len; i++)
    {
        BSP_PushFIFOByte(fifo, buff[i]);
    }
}
