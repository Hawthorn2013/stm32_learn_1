#ifndef _OLED_H_
#define _OLED_H_

#include <stdint.h>

#define OLED_SEG (128)
#define OLED_PAGE (8)
#define OLED_COM (64)

//提供的接口
void OLED_Init(void);
void OLED_Fill(uint8_t data);
void OLED_Print6x8Str(uint8_t x, uint8_t y, const uint8_t str[]);

#endif
