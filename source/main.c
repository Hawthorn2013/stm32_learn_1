#include <stm32f10x_conf.h>

void Delay_us(uint32_t us)
{
	volatile uint16_t us_i = us;
	while(us_i--)
		;
}

int main(void)
{
	volatile int i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = 0XFFFF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	while(1)
  {
		GPIO_Write(GPIOA, 0xFFFF);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		GPIO_Write(GPIOA, 0);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		Delay_us(0xffffffff);
		i++;
	}
}
