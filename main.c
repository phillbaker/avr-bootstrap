/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 * 
 * Code base from http://www.instructables.com/id/AVRArduino-RFID-Reader-with-UART-Code-in-C/?ALLSTEPS
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "m_general.h"
#include "m_rfid.h"

static void RFID_init(void);
static void RFID_ena(uint8_t ena);
static uint8_t ValidTag(void);
static void TunedDelay(uint16_t);

#define RFID_IN		7 //D7
#define RFID_OUT	6 //D6

// Values taken from SoftSerial from Mikal Hart; 16MHz clock speed
#define CENTER_DELAY			471
#define INTRABIT_DELAY			950

static char	RFID_tag[12];
static volatile uint8_t bDataReady;
static volatile uint8_t rxIdx;

int main(void) {
  PLLCSR = 0x12; // 0001 0010 For a 16MHz clock
  USART_init(BAUD_9600);	
	m_wait(2000);	// pause 2s //?
  sei();
  
	//USART_tx_S("USART Initialized!\n");

	// Setup RFID pins on port 
	RFID_init();
	RFID_ena(1);
	TunedDelay(947);	// center on serial pulse

	char ibuff[2];
	for (;;) {
		if (bDataReady) {
#ifdef __DEBUG__
			USART_tx_S("Start byte: ");
			USART_tx_S(itoa(RFID_tag[0],&ibuff[0],16));
			ibuff[0] = 0; ibuff[1] = 0;
			USART_tx_S("\nStop  byte: ");
			USART_tx_S(itoa(RFID_tag[11],&ibuff[0],16));
#endif
			if ( ValidTag() ) {
				// USART_tx_S("\nRFID Tag: ");
				//        for(uint8_t x = 1; x < 11; x++) {
				//          USART_tx_S(itoa(RFID_tag[x],ibuff,16));
				//          if (x != 10)
				//            USART_tx(':');
				//        }
				//        USART_tx_S("\n");
			}
			rxIdx = 0;
			bDataReady = 0;
		}
	}//end main for
}

void RFID_init() {
	bDataReady = 0;
	rxIdx = 0;
	
	// RFID_IN input from RFID Reader SOUT
  clear(DDRB, RFID_IN);//pin input

	//BSET(PCICR,PCIE2);	// pin change interrupt control register pcie2
	//BSET(PCMSK2,PCINT18); // enable pin change interrupt for PCINT18 (PD2)
	//BSET(SREG,7);		// Set SREG I-bit
	
}

// this is the interrupt handler
ISR(PCINT2_vect) {
	if (BCHK(PIND,RFID_IN))	// Start bit goes low
		return;
	uint8_t	bit = 0;
	TunedDelay(CENTER_DELAY);		// Center on start bit
	for (uint8_t x = 0; x < 8; x++) {
		TunedDelay(INTRABIT_DELAY);	// skip a bit, brother...
		if (BCHK(PIND,RFID_IN))
			BSET(bit,x);
		else
			BCLR(bit,x);	
	}
	TunedDelay(INTRABIT_DELAY);		// skip stop bit
	RFID_tag[rxIdx] = bit;
	++rxIdx;
	if (rxIdx == 12)
		bDataReady = 1;
}

void RFID_ena(uint8_t ena) {
	if (ena)
		BCLR(PORTD,RFID_ENA);
	else
		BSET(PORTD,RFID_ENA);
}

void TunedDelay(uint16_t delay) {
  uint8_t tmp=0;

  asm volatile("sbiw    %0, 0x01 \n\t"
    "ldi %1, 0xFF \n\t"
    "cpi %A0, 0xFF \n\t"
    "cpc %B0, %1 \n\t"
    "brne .-10 \n\t"
    : "+r" (delay), "+a" (tmp)
    : "0" (delay)
    );

}

void CriticalSegment(uint8_t ena) {
	static uint8_t sreg;
	if (ena)
	{
		sreg = SREG;	// save off our global status register
		cli();			// stop global interrupts
	} else
	{
		SREG = sreg;	// restore global interrupt flag (and any other prev settings)
	}
}

uint8_t ValidTag() {
	// Format: [Start Byte|RFID|Stop Byte]
	// Format: 0x0A[10 hex values]0x0D
	// All we're doing here is checking to see if this is a valid RFID
	// tag or if we got random noise.
	if (RFID_tag[0] == 0x0A)
		return 1;

	return 0;
}
