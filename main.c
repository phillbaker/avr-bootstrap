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
  //this expects usart input to be on pin D2 (output from the rfid reader)
  usart_rx_init();
  
  char tag[TAG_LENGTH + 1];//do length + checksum bit
  while(true) {
    //block and read until success
    m_rfid_bread_tag(tag);
  }
}
