#include <stm32f10x_conf.h>
#include <stdio.h>
#include <string.h>
#include "oled.h"

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

void OLED_Reset()
{
    GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
    Delay_us(1000);
    GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET);
    Delay_us(1000);
}

void OLED_SendCmd(uint8_t cmd)
{
    GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_RESET);       //指定发送cmd
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_RESET);       //建立片选
    while((SPI2->SR & SPI_I2S_FLAG_TXE) == RESET);
    SPI2->DR = cmd;
    while((SPI2->SR & SPI_I2S_FLAG_RXNE) == RESET);
    (void)(SPI2->DR);  
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET);         //清除片选
}

void OLED_SendData(uint8_t data)
{
    GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_SET);         //指定发送data
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_RESET);
    while((SPI2->SR & SPI_I2S_FLAG_TXE) == RESET);      //等待发送区空  
    SPI2->DR = data;                                    //发送一个byte  
    while((SPI2->SR & SPI_I2S_FLAG_RXNE) == RESET);     //等待接收完一个byte 
    (void)(SPI2->DR);                                   //返回收到的数据   
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET);
}

int main(void)
{
    volatile int i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
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
    //SPI配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    SPI_Cmd(SPI2, DISABLE);            //必须先禁能,才能改变MODE
    SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex;  //两线全双工
    SPI_InitStructure.SPI_Mode =SPI_Mode_Master;       //主
    SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b;      //8位
    SPI_InitStructure.SPI_CPOL =SPI_CPOL_Low;        //CPOL=1时钟悬空低
    SPI_InitStructure.SPI_CPHA =SPI_CPHA_1Edge;       //CPHA=1 数据捕获第1个
    SPI_InitStructure.SPI_NSS =SPI_NSS_Soft;        //软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_256;  //2分频
    SPI_InitStructure.SPI_FirstBit =SPI_FirstBit_MSB;      //高位在前
    SPI_InitStructure.SPI_CRCPolynomial =7;        //CRC7
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_11| GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    OLED_Reset();
    OLED_Init();
    OLED_Fill(0x00);
    while(1)
    {
        char dst[50];
        sprintf(dst, "%d + %f = %f\r\n", i, 7.4, i + 7.4);
        Send_To_Console(dst, strlen(dst));
        i++;
        OLED_Print6x8Str(0, 0, (const uint8_t *)"11122233\r\n44\r55566\n77788\r99\n000");
    }
}
