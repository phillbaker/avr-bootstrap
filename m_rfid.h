/* Name: m_rfid.h
 * Author: Phill Baker
 * Copyright: All rights reserved, 2012.
 * License: MIT License
 * 
 * http://www.seeedstudio.com/depot/125khz-rfid-module-uart-p-171.html
 * http://www.gumbolabs.org/2009/10/17/parallax-rfid-reader-arduino/ <- this code was helpful
 * http://www.kanda.com/AVR-C-Code-UART.php
 * 
 * TODO: interupt driven code, non-blocking check for bytes available in the queue
 * http://www.openhardwarehub.com/assets/projects/files/59/uart.c?1318091592
 */

#define START_BYTE 0x2 
#define STOP_BYTE 0x3
/**
* Max length of RFID tag.
**/
#define TAG_LENGTH 10      
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

/**
* Non-blocking function that returns 0 on failure and 1 on success; sets c to the read byte.
**/
int usart_receive_byte(unsigned char* c);

/**
* Boolean whether buffer has a byte or not.
**/
int usart_has_byte(void);

/**
* Computes the checksum of the array and returns 0 on failure and 1 on success.
**/
int usart_checksum(unsigned char c[]);

/**
* Non-block function that returns -1 on non receiving the start_byte and 1 on the start byte.
**/
int usart_start_byte(void);
