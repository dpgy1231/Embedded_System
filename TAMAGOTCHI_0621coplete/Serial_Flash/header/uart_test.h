/*
 * uart_test.h
 *
 *  Created on: 2014. 12. 9.
 *      Author: Administrator
 */

#ifndef UART_TEST_H_
#define UART_TEST_H_
#include <stdint.h>

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
char uartgetchar(uint32_t ui32Base);
char uartget(uint32_t ui32Base);
char uartget_delay(int delay);

#define ESC		0x1B

#endif /* UART_TEST_H_ */
