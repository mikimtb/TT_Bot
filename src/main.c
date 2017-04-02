/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/



#include <stm32f4xx.h>
#include "uart_user.h"

int main(void)
{
	uart_init(256000);

	while (1)
	{
		while (uart_bkbhit())
		{
			uart_bputc(uart_bgetc());
		}
	}
}
