/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"

int main(void)
{
    /* insert your hardware initialization here */
    m_usb_init();
    /* program variables */
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
