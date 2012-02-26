/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_rfid.h"

#define TAG_LENGTH 10      //Max length of RFID tag
#define ITERATION_LENGTH 2000 //time, in ms, given to the user to move hand away

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
    m_red(OFF);
    int tag_bytes_read = 0;
    c = usart_breceive();
    //wait for start byte
    if(c != START_BYTE) {
      continue;
    }
    m_red(ON);
    while(tag_bytes_read < TAG_LENGTH + 1) {
      c = usart_breceive();
      if(c == STOP_BYTE)
        break;
      tag[tag_bytes_read] = c;
      tag_bytes_read++;
    }
    
    if(m_usb_isconnected()) {
      //each card consists of 13 bits: start bit, 10 data bits, checksum, end flag; the 10 data bits are pairs of hex numbers
      //take 13 bits out of 24
      
      int i;
      //least significant bit at lowest value
      for(i = 0; i < TAG_LENGTH; i++) {
        m_usb_tx_char(tag[i]);
        //m_usb_tx_string(" ");
      }
    
      m_usb_tx_string("\n");
    }
    tag_bytes_read = 0;
  }
}
