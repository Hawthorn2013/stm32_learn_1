#ifndef _BSP_H_
#define _BSP_H_

#include "string.h"
#include "stm32f1xx_hal.h"

#define BSP_USART_FIFO_LEN (512)

typedef struct
{
    uint32_t head;
    uint32_t tail;
    uint8_t data[BSP_USART_FIFO_LEN];
} BSP_FIFO;

extern volatile uint8_t USART1_RX_data;
extern volatile uint8_t USART2_RX_data;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;

void Delay_us(uint32_t us);
void Send_To_Console(const char *data, uint32_t len);
void OLED_Reset(void);
void OLED_SendCmd(uint8_t cmd);
void OLED_SendData(uint8_t data);
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen);
int transport_getdata(unsigned char* buf, int count);
int transport_getdatanb(void *sck, unsigned char* buf, int count);
int transport_open(char* host, int port);
int transport_close(int sock);
void BSP_InitFIFO(BSP_FIFO *fifo);
uint32_t BSP_GetFIFOAvailableLen(BSP_FIFO *fifo);
uint32_t BSP_PopFIFOByte(BSP_FIFO *fifo, uint8_t *data);
uint32_t BSP_PopFIFO(BSP_FIFO *fifo, uint8_t *buff, uint32_t len);
void BSP_PushFIFOByte(BSP_FIFO *fifo, uint8_t data);
void BSP_PushFIFO(BSP_FIFO *fifo, uint8_t *buff, uint32_t len);

#endif
