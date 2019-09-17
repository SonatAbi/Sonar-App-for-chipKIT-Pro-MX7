// ECE 2534:        myUART.h
// Purpose:         User-generated UART helper function prototypes.
// Resources:       UART only
// Written by:      JST
// Last modified:   06 February 2017 (by JST)

// Includes
#include <plib.h>

#ifndef MYUART_H
#define MYUART_H

///////////////////////////////////////////////////////////////////////////////
// Function:     initUART
// Description:  Initializes UART on the PIC32 for transmission and reception
//               using a data frame of 8 data bits, no parity, and 1 stop bit.
//               Also turns on the UART.
// Parameters:   uart        - The Cerebot UART module.
//               sourceClock - The Cerebot peripheral clock frequency
//               dataRate    - the baud rate of the UART channel
// Return value: N/A

void initUART(UART_MODULE uart, unsigned int sourceClock, unsigned int dataRate);

//////////////////////////////////////////////////////////////////////////
// Function:     UARTReceiveByte
// Description:  Receives a byte from UART. 
//               It waits until it accomplishes this goal.
// Parameters:   uart          - The Cerebot UART module.
// Return value: byte_received - the byte received.

unsigned char UARTReceiveByte(UART_MODULE uart);

//////////////////////////////////////////////////////////////////////////
// Function:     UARTSendByte
// Description:	 Transmits a byte via UART.
//               It waits until it accomplishes this goal.
// Parameters:   uart      - The Cerebot UART module.
//               byte_sent - The byte transmitted via UART.
// Return value: N/A

void UARTSendByte(UART_MODULE uart, unsigned char byte_sent);

//////////////////////////////////////////////////////////////////////////
// Function:     UARTSendString
// Description:  Transmits a string via UART. 
//               It waits until it accomplishes this goal.
// Parameters:   uart   - The Cerebot UART module.
//               string - A pointer to the string being transmitted.
// Return value: N/A

void UARTSendString(UART_MODULE uart, const char* string);

#endif
