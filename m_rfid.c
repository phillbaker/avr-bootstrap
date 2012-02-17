#include "m_usb.h"

#include <avr/pgmspace.h>
#include <stdio.h>
#include <limits.h>

static void uart_2400(void);
static void uart_9600(void);
static void uart_38400(void);

char * baseconv(unsigned int num, int base) {
	static char retbuf[sizeof(int)*CHAR_BIT+1];
	char * p;

	if (base < 2 || base > 16) return NULL;
	p = &retbuf[sizeof(retbuf)-1];
	*p = '\0';

	do {
		*--p = "01234567890abcdef"[num % base];
		num /= base;
	} while(num != 0);

	return p;
}

// Initialize the UART to b Bd, tx/rx, 8N1, asynchronous mode.
// TODO: Add support for bit size, parity, stop bits, etc
void USART_init(uint8_t b) {
	switch(b)
	{
	case BAUD_2400:		uart_2400(); break;
	case BAUD_38400:	uart_38400(); break;
	case BAUD_9600:
	default:
		uart_9600(); break;
	}

	// Enable Rx/Tx in register UCSR0B
	UCSR0B 	= (1<<RXEN0)|(1<<TXEN0);
}

// USART_transmit
// Send data to USART buffer for transmission
void USART_tx(unsigned char data) {
	// Check UDRE0 in UCSR0A.  If 1, then buffer empty.
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = data;
}

void USART_tx_S(const char * data) {
	while(*data != 0x00)
		USART_tx(*(data++));
}

void 	USART_tx_P(const char * data) 
{
	while(pgm_read_byte(data) != 0x00)
		USART_tx(pgm_read_byte(data++));
}


// USART_receive
// 
unsigned char USART_rx(void) {
	loop_until_bit_is_set(UCSR0A,RXC0);
	return UDR0;
}

static void uart_2400(void) {
	#define BAUD 2400
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
}


static void uart_9600(void) {
	#undef BAUD
	#define BAUD 9600
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
}

static void uart_38400(void) {
	#undef BAUD // avoid compiler warning
	#define BAUD 38400
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
}

