#include "oled.h"

//OLED需要的板集接口
extern void OLED_Reset(void);
extern void OLED_Init(void);
extern void OLED_SendCmd(uint8_t cmd);
extern void OLED_SendData(uint8_t data);

void OLED_Init(void)
{
    OLED_SendCmd(0xae);//--turn off oled panel
	OLED_SendCmd(0x00);//---set low column address
	OLED_SendCmd(0x10);//---set high column address
	OLED_SendCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_SendCmd(0x81);//--set contrast control register
	OLED_SendCmd(0xcf); // Set SEG Output Current Brightness
	OLED_SendCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_SendCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_SendCmd(0xa6);//--set normal display
	OLED_SendCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_SendCmd(0x3f);//--1/64 duty
	OLED_SendCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_SendCmd(0x00);//-not offset
	OLED_SendCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_SendCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_SendCmd(0xd9);//--set pre-charge period
	OLED_SendCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_SendCmd(0xda);//--set com pins hardware configuration
	OLED_SendCmd(0x12);
	OLED_SendCmd(0xdb);//--set vcomh
	OLED_SendCmd(0x40);//Set VCOM Deselect Level
	OLED_SendCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_SendCmd(0x02);//
	OLED_SendCmd(0x8d);//--set Charge Pump enable/disable
	OLED_SendCmd(0x14);//--set(0x10) disable
	OLED_SendCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_SendCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_SendCmd(0xaf);//--turn on oled panel
}

void OLED_Fill(uint8_t data)
{
	uint8_t y, x;
    
	for(y = 0; y < 8; y++)
	{
		OLED_SendCmd(0xb0 + y);
		OLED_SendCmd(0x01);
		OLED_SendCmd(0x10);
		for(x = 0 ;x < 128; x++)
			OLED_SendData(data);
	}
}
