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
			
void USART2_IRQHandler(void)
{
	/** Check whether receive interrupt is triggered USART2 IRQ handler */
	if (USART2->SR & USART_FLAG_RXNE)
	{
		USART2->SR &= ~USART_FLAG_RXNE;		/** Clear interrupt flag */

		uint16_t result_word = USART_ReceiveData(USART2);
		uint8_t result_byte = (uint8_t) result_word;
	}
}

int main(void)
{
	uart_init(115200);
	uart_printf(USART2,"USART ready");

	for(;;);
}
