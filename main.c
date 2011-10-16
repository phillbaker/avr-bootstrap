/* Name: main.c
 * Author: Phill Baker (phills@seas.upenn.edu)
 * Copyright: (c) 2011, All Rights Reserved.
 * License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0) (http://creativecommons.org/licenses/by-nc-sa/3.0/)
 */

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"

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
