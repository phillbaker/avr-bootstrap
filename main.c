/* Name: main.c
 * Author: Phill baker
 * Copyright: (c) 2011, All Rights Reserved.
 * License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0) (http://creativecommons.org/licenses/by-nc-sa/3.0/)
 */

#include <avr/io.h>

int main(void)
{
		DDRE |= 1<<6;
		PORTE &= !(1<<6);
    /* insert your hardware initialization here */
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
