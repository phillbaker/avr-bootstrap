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

void usart_init(unsigned int baud);
unsigned char usart_receive(void);

int main(void) {
  m_clockdivide(0); // 16 MHz
  usart_init(9600);
  //D2 	RXD1 	USART receive
  clear(DDRD, 2);//pin 4 input
  //D3 	USART transmit
  //D5 	USART external clock in/out
  
  //Receive Enable (RXENn) bit in the UCSRnB Register to one
  set(UCSR1A, RXEN1);
  
  unsigned char c;
  while(true) {
    c = usart_receive();
  }
}

void usart_init(unsigned int baud) {
  //9600bps, No parity ,8 databits, 1 stopbit (assumed no flow control)
  
  /* Set baud rate */ 
  UBRR1H = (unsigned char)(baud>>8);
  UBRR1L = (unsigned char)baud;
  /* Enable receiver and transmitter */
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);
  /* Set frame format: 8 data, 1stop bit */
  UCSR1C = (0<<USBS1)|(3<<UCSZ10);
}

unsigned char usart_receive(void) {
  //Wait for data to be received
  while (!(UCSR1A & (1<<RXC1)))
    ; //loop and wait
  //Get and return received data from buffer
  return UDR1;
}