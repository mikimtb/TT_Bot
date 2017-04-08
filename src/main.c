/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/



#include "stm32f4xx.h"
#include "uart_user.h"
#include "gpio_user.h"

int main(void)
{
	uint8_t c;

	uart_init(115200);
	gpio_init();

	GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
	while (1)
	{
		while (uart_bkbhit())
		{
			uart_bgetc(&c);
			uart_bputc(c);
		}
	}
}
