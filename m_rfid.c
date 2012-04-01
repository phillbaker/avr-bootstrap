#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "m_rfid.h"
#include "m_general.h"

void usart_rx_init(void) {
  //Receive Enable (RXENn) bit in the UCSRnB Register to one
  set(UCSR1A, RXEN1);
  
  //9600bps, No parity ,8 databits, 1 stopbit (assumed no flow control)
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
  while (!usart_has_byte())
    ; //loop and wait
  //Get and return received data from buffer
  return UDR1;
}

int usart_has_byte(void) {
  if(UCSR1A & (1 << RXC1)) {
    return 1;
  }
  return 0;
}

int usart_receive_byte(unsigned char* c) {
  if(usart_has_byte()) {
    *c = UDR1;
    return 1;
  }
  return 0;
}

int usart_start_byte(void) {
  //check for start byte
  unsigned char c;
  int ret = usart_receive_byte(&c);
  if(ret && c == START_BYTE) {
    return 1;
  }
  return 0;
}

int usart_checksum(unsigned char tag[]) {
  //TODO doesn't like this, returns negative lengths...
  // int len = strlen(tag);
  // if((int)len != 12) {//reject if we don't have the right number
  //   return len;
  // }
  //"0300BFB417" => "1F" checksum, 0x03 ^ 0x00 ^ 0xBF ^ 0xB4 ^ 0x17
  
  //take pairs of the string, convert them to hex numbers, XOR the 5 pairs
  int b1;
  int b2;
  int b3;
  int b4;
  int b5;
  int cb;
  sscanf(tag, "%2x%2x%2x%2x%2x%2x", &b1, &b2, &b3, &b4, &b5, &cb);
  int checksum = b1 ^ b2 ^ b3 ^ b4 ^ b5;
  return checksum == cb;
}