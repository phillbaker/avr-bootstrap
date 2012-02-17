#ifndef m_rfid__
#define m_rfid__

#include "m_general.h"
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// This just makes my life easier
#define BIT16(b) ((unsigned long)0x00000001 << (b))
#define	BIT8(b) (0x01 << (b))
char * baseconv(unsigned int num, int base);

// from AVR035: Efficient C Coding for AVR 
#define BSET(ADDRESS,BIT) (ADDRESS |= (1<<BIT)) 
#define BCLR(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT)) 
#define BTOG(ADDRESS,BIT) (ADDRESS ^= (1<<BIT)) 
#define BCHK(ADDRESS,BIT) (ADDRESS &  (1<<BIT)) 

#define BMSET(x,y) (x |= (y)) 
#define BMCLR(x,y) (x &= (~y)) 
#define BMTOG(x,y) (x ^= (y)) 
#define BMCHK(x,y) (x & (y)) 

#define BMPRINT(x) printf("[%8s]\n",baseconv(x,2));

#pragma once//TODO what?

//#define UART_BAUD 9600
#define BAUD_9600	1
#define BAUD_2400	2
#define BAUD_38400	3

//Perform UART startup initialization.
void	USART_init(uint8_t b);

// Send one character to the UART.
unsigned char	USART_rx(void);

// Receive one character from the UART.  The actual reception is
// line-buffered, and one character is returned from the buffer at
// each invokation.
void	USART_tx(unsigned char);
void 	USART_tx_S(const char *);

void 	USART_tx_P(const char *);
