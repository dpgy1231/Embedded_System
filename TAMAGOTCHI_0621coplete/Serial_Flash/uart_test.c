/*
 * uart_test.c
 *
 *  Created on: 2014. 12. 9.
 *      Author: Administrator
 */
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "header/uart_test.h"

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
    	while(!UARTSpaceAvail(UART0_BASE));
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

char uartgetchar(uint32_t ui32Base)
{
	while(!UARTCharsAvail(UART0_BASE));

	return(char)UARTCharGetNonBlocking(UART0_BASE);
}

char uartget(uint32_t ui32Base)
{
	return(char)UARTCharGetNonBlocking(UART0_BASE);
}

char uartget_delay(int delay)
{
	char ret;

	while(delay--)
	{
		ret = (char)UARTCharGetNonBlocking(UART0_BASE);
		if(ret == ESC) break;

	}
	return ret;
}



