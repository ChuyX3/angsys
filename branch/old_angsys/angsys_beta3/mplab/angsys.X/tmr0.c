/* 
 * File:   tmr0.c
 * Author: Jesus Rocha
 *
 * Created on April 16, 2018, 12:22 PM
 */

#include <angsys.h>


void tmr0_initialize(byte config)
{
    T0CON = config & 0X7F;
}

void tmr0_start(void)
{
    T0CONbits.TMR0ON = true;
}

void tmr0_stop(void)
{
    T0CONbits.TMR0ON = false;
}

void tmr0_reset(void)
{
    TMR0L = 0;
    TMR0H = 0; 
}

void tmr0_set(word value)
{
    TMR0L = (word)value;
    TMR0H = (word)(value >> 8);
}

word tmr0_get(void)
{
    return ((word)TMR0H << 8) | TMR0L;
}

void tmr0_set_interrupt(bool enabled)
{
    INTCONbits.T0IF = false;
    INTCONbits.T0IE = enabled;
}

bool tmr0_get_interrupt(void)
{
    return INTCONbits.T0IE;
}
