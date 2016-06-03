#include <stm32f10x_conf.h>
#include <stdio.h>
#include <string.h>

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
        USART_SendData(USART1, data[i]);
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//等待发送结束
    }
}

int main(void)
{
    volatile int i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART 初始化设置
    USART_DeInit(USART1);
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);                    //使能串口
    while(1)
    {
        char dst[50];
        sprintf(dst, "%d + %f = %f\r\n", i, 7.4, i + 7.4);
        Send_To_Console(dst, strlen(dst));
        i++;
    }
}
