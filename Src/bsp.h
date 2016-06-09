#ifndef _BSP_H_
#define _BSP_H_

#include "stm32f1xx_hal.h"

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

#endif
