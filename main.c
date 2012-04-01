/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "m_general.h"
#include "m_rfid.h"

int main(void) {
  m_clockdivide(0); // 16 MHz

  //this expects usart input to be on pin D2 (output from the rfid reader)
  usart_rx_init();
  //D2 	RXD1 	USART receive
  clear(DDRD, 2);
  
  while(true) {
    m_red(OFF);
    
    if(!usart_start_byte()) {
      continue;
    }
    
    m_red(ON);
    int tag_bytes_read = 0;
    unsigned char c;
    unsigned char tag[TAG_LENGTH + 2];//do length + checksum bytes
    while(tag_bytes_read < TAG_LENGTH + 2) {
      c = usart_breceive();
      tag[tag_bytes_read] = c;
      tag_bytes_read++;
    }
    
    if(usart_checksum(tag)) {
      m_red(ON);
    }
  }
}
