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

uint8_t send_flag = 0;
uint8_t c = 0;

void USART2_IRQHandler(void)
{
	/** Check whether receive interrupt is triggering USART2 IRQ handler */
	if (USART2->SR & USART_FLAG_RXNE)
	{
		USART2->SR &= ~USART_FLAG_RXNE;					/** Clear receive interrupt flag */

		uint16_t result_word = USART_ReceiveData(USART2);
		c = (uint8_t) result_word;
		send_flag = 1;
	}
	/** Check whether transmit interrupt is triggering USART2 IRQ handler */
	else if (USART_FLAG_TXE)
	{
		USART2->CR1 &= ~USART_FLAG_TXE;					/** Clear transmit interrupt flag */
		uart_WriteByte(USART2, c);
	}
}

int main(void)
{
	uart_init(115200);
	uart_printf(USART2,"USART ready");

	for(;;)
	{
		if(send_flag == 1)
		{
			/*uint8_t c = uart_ReadByte(USART2);*/
			USART2->CR1 |= USART_FLAG_TXE;				/** Enable transmit interrupt */
			send_flag = 0;
		}
	}
}
