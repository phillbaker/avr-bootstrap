#include <avr/io.h>
#include <avr/interrupt.h>
#include "m_rfid.h"

void usart_rx_init(void) {
  //Set baud rate
  UBRR1H = (unsigned char)(UBRR_VAL>>8);
  UBRR1L = (unsigned char)UBRR_VAL;
  //Enable receiver and transmitter
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);
  //Set frame format: 8 data, 1stop bit (stop bits ignored in reading)
  // Use 8-bit characters
  UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
}

unsigned char usart_breceive(void) {
  //Wait for data to be received
  while (!(UCSR1A & (1<<RXC1)))
    ; //loop and wait
  //Get and return received data from buffer
  return UDR1;
}