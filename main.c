/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 * 
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "m_general.h"
#include "m_usb.h"

void usart_init(unsigned int baud);
unsigned char usart_receive(void);

int main(void) {
  m_clockdivide(0); // 16 MHz
  m_usb_init();
  usart_init(9600);
  //D2 	RXD1 	USART receive
  clear(DDRD, 2);
  //D3 	USART transmit
  //D5 	USART external clock in/out
  
  //Receive Enable (RXENn) bit in the UCSRnB Register to one
  set(UCSR1A, RXEN1);
  
  unsigned char c;
  while(true) {
    int i = 0;
    char buff[3];
    while(i < 2) {
      c = usart_receive();
      buff[i] = c;
      i++;
    }
    if(m_usb_isconnected()) {
      //each card consists of 13 bits: start bit, 10 data bits, checksum, end flag; the 10 data bits are pairs of hex numbers
      //take 13 bits out of 24
      int data[13];
      data[0] = buff[0] >> 2;
      
      m_usb_tx_int(data[0]);
      m_usb_tx_string(" ");
      
      m_usb_tx_char(buff[0]);
      m_usb_tx_char(buff[1]);
      m_usb_tx_char(buff[2]);
      
      //char str[1];
      //sprintf(str, "%c", c);
      //m_usb_tx_int((int)c);
      //m_usb_tx_string(&str);
    
      m_usb_tx_string("\n");
    }
    i = 0;
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