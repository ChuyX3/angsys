/* 
 * File:   tmr2.c
 * Author: Jesus Rocha
 *
 * Created on April 16, 2018, 12:22 PM
 */

#include <angsys.h>

void tmr2_initialize(byte config)
{
    T2CON = config & 0X7B;
}

void tmr2_start(void)
{
    T2CONbits.TMR2ON = true;
}

void tmr2_stop(void)
{
    T2CONbits.TMR2ON = false;
}

void tmr2_reset(void)
{
    TMR2 = 0;
    PR2 = 0; 
}

void tmr2_set(byte value)
{
    TMR2 = value;
}

byte tmr2_get(void)
{
    return TMR2;
}

void tmr2_period_set(byte value)
{
    PR2 = value;
}

byte tmr2_period_get(void)
{
    return PR2;
}
