#include <avr/io.h>
#include <avr/interrupt.h>
#include "m_rfid.h"

void usart_rx_init(void) {
  m_clockdivide(0); // 16 MHz
  
  //D2 	RXD1 	USART receive, enable digital in
  clear(DDRD, 2);
  //D3 	USART transmit
  //D5 	USART external clock in/out
  
  //Receive Enable (RXENn) bit in the UCSRnB Register to one
  set(UCSR1A, RXEN1);
  
  //Set baud rate
  UBRR1H = (unsigned char)(UBRR_VAL>>8);
  UBRR1L = (unsigned char)UBRR_VAL;
  //Enable receiver and transmitter
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);
  //Set frame format: 8 data, 1stop bit (stop bits ignored in reading)
  // Use 8-bit characters
  UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
}

int m_rfid_bread_byte(unsigned char* c) {
  //Wait for data to be received
  int res = m_rfid_read_byte(c);
  while (res < 0)
    ; //loop and wait
  //Get and return received data from buffer
  return 1;
}

int m_rfid_read_byte(unsigned char* c) {
  //Wait for data to be received
  if (!(UCSR1A & (1<<RXC1)))
    return -1; //loop and wait
  //Get and use received data from buffer
  *c = UDR1;
  return 1;
}

int m_rfid_bread_tag(char buffer[]) {
  //loop until a tag is found
  while(true) {
    int ret = m_rfid_read_tag(buffer);
    if(ret < 0) {
      continue;
    }
    //else return the value on success
    return ret;
  }
}

//read tag if this is a start byte or -1 for not the beginning
int m_rfid_read_tag(char buffer[]) {
  unsigned char c;
  int res = m_rfid_read_byte(&c);
  //wait for start byte
  if(c != START_BYTE) {
    return -1;
  }
  //if we get start byte, read the tag
  return m_rfid_get_tag_bytes(buffer);
}

//must be on first byte of the tag
int m_rfid_get_tag_bytes(char buffer[]) {
  unsigned char c;
  int tag_bytes_read = 0;
  //read tag_length + 1 to get checksum byte and stop byte
  while(tag_bytes_read < TAG_LENGTH + 1) {
    int res = m_rfid_bread_byte(&c);
    if(c == STOP_BYTE) //TODO if we get past TAG_LENGTH and there's no STOP_BYTE return -1; also do noise check via checksum byte
      break;
    buffer[tag_bytes_read] = c;
    tag_bytes_read++;
  }
  return 1;
}