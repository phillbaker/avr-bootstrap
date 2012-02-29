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

int main(void) {
  //this expects usart input to be on pin D2 (output from the rfid reader)
  usart_rx_init();
  m_usb_init();
  
  char tag[TAG_LENGTH + 1];//do length + checksum bit
  int count = 0;
  while(true) {
    //keep a count and timeout if we haven't seen a tag in a while
    m_wait(200);
    m_green(OFF);
    count += 1;
    if(m_usb_isconnected()) {
      m_usb_tx_int((int)count);
      m_usb_tx_string("\n");
    }
    if(count > 10) { //choose a relatively high, arbitrary number so we can miss a few every once in a while
      m_red(OFF);
    }
    
    int res = m_rfid_read_tag(tag);
    if(res < 0) {
      continue;
    }
    //m_wait(1000);
    if(m_usb_isconnected()) {
      //each card consists of 13 bits: start bit, 10 data bits, checksum, end flag; the 10 data bits are pairs of hex numbers
      //take 13 bits out of 24
      
      int i;
      //least significant bit at lowest value
      for(i = 0; i < TAG_LENGTH; i++) {
        m_usb_tx_char(tag[i]);
        //m_usb_tx_string(" ");
      }
    
      // m_usb_tx_string(" ");
      // m_usb_tx_uint(count);
      m_usb_tx_string("\n");
    }
    
    count = 0;
    m_red(ON);
    m_green(ON);
  }
}
