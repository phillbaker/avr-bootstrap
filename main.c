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

#define TAG_LENGTH 10      //Max length of RFID tag
#define START_BYTE 0x29 
#define STOP_BYTE 0x39
#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1) //use F_CPU defined at command line of compile flags

#define ITERATION_LENGTH 2000 //time, in ms, given to the user to move hand away

void usart_rx_init(void);
unsigned char usart_receive(void);

char tag[TAG_LENGTH + 1];//do length + checksum bit

int main(void) {
  m_clockdivide(0); // 16 MHz
  m_usb_init();
  usart_rx_init();
  //D2 	RXD1 	USART receive
  clear(DDRD, 2);
  //D3 	USART transmit
  //D5 	USART external clock in/out
  
  //Receive Enable (RXENn) bit in the UCSRnB Register to one
  set(UCSR1A, RXEN1);
  
  unsigned char c;
  while(true) {
    // int tag_bytes_read = 0;
    c = usart_receive();
    // //wait for start byte
    // if(c != START_BYTE) {
    //   continue;
    // }
    // m_red(ON);
    // while(tag_bytes_read < TAG_LENGTH + 1) {
    //   c = usart_receive();
    //   if(c == STOP_BYTE)
    //     break;
    //   tag[tag_bytes_read] = c;
    //   tag_bytes_read++;
    // }
    
    if(m_usb_isconnected()) {
      //each card consists of 13 bits: start bit, 10 data bits, checksum, end flag; the 10 data bits are pairs of hex numbers
      //take 13 bits out of 24
      
      // m_usb_tx_int(data[0]);
      // m_usb_tx_string(" ");
      // 
      // m_usb_tx_char(buff[0]);
      // m_usb_tx_char(buff[1]);
      // m_usb_tx_char(buff[2]);
      
      //char str[1];
      //sprintf(str, "%c", c);
      m_usb_tx_int((int)c);
      // int i;
      // for(i = 0; i < TAG_LENGTH; i++) {
      //   m_usb_tx_int((int)tag[i]);
      //   m_usb_tx_string(" ");
      // }
      //m_usb_tx_string(&str);
    
      m_usb_tx_string("\n");
    }
    // tag_bytes_read = 0;
  }
}

void usart_rx_init(void) {
  //9600bps, No parity ,8 databits, 1 stopbit (assumed no flow control)
  
  /* Set baud rate */ 
  UBRR1H = (unsigned char)(UBRR_VAL>>8);
  UBRR1L = (unsigned char)UBRR_VAL;
  /* Enable receiver and transmitter */
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);
  /* Set frame format: 8 data, 1stop bit (stop bits ignored in reading) */
  //UCSR1C = (0<<USBS1)|(3<<UCSZ10);
  // Use 8-bit characters
  UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
}

/**
* Blocking function that returns char value in buffer.
*/
unsigned char usart_receive(void) {
  //Wait for data to be received
  while (!(UCSR1A & (1<<RXC1)))
    ; //loop and wait
  //Get and return received data from buffer
  return UDR1;
}