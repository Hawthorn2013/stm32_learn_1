#ifndef _BSP_H_
#define _BSP_H_

#include <stm32f10x_conf.h>

void Init_STM32(void);
void Delay_us(uint32_t us);
void Send_To_Console(const char *data, uint32_t len);
void OLED_Reset(void);
void OLED_SendCmd(uint8_t cmd);
void OLED_SendData(uint8_t data);

#endif
