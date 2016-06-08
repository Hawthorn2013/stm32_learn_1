#include <stm32f10x_conf.h>
#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "oled.h"

int main(void)
{
    volatile int i = 0;
    
    Init_STM32();
    OLED_Reset();
    OLED_Init();
    OLED_Fill(0x00);
    while(1)
    {
        char dst[50];
        sprintf(dst, "%d + %f = %f\r\n", i, 7.4, i + 7.4);
        Send_To_Console(dst, strlen(dst));
        i++;
        //OLED_SetPos(0, 0);
        OLED_Print6x8Str((const uint8_t *)"1111111111111111111111111");
        //OLED_UpdateMemory();
        USART_SendData(USART2, 0x15);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);//等待发送结束
    }
}
