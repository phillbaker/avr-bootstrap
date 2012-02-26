/* Name: m_rfid.h
 * Author: Phill Baker
 * Copyright: All rights reserved, 2012.
 * License: MIT License
 * 
 * http://www.seeedstudio.com/depot/125khz-rfid-module-uart-p-171.html
 * http://www.gumbolabs.org/2009/10/17/parallax-rfid-reader-arduino/ <- this code was helpful
 * 
 * TODO: interupt driven code, non-blocking check for bytes available in the queue
 */

#define START_BYTE 0x2 
#define STOP_BYTE 0x3
#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1) //use F_CPU defined at command line of compile flags

///////////////////////////////////////////////////////////////////////////////
////Public functions
///////////////////////////////////////////////////////////////////////////////
/**
* Init the USART connection: 9600bps, No parity ,8 databits, 1 stopbit (assumed no flow control)
**/
void usart_rx_init(void);
/**
* Blocking function that returns char value in buffer. (Ie blocking receive)
**/
unsigned char usart_breceive(void);
