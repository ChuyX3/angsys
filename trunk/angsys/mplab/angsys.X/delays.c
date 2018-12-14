/* 
 * File:   pwm.c
 * Author: Jesus Rocha
 *
 * Created on March 5, 2018, 12:22 PM
 */

#include <angsys.h>

float ang_tick_s = 1;
void delays_initialize(float FOSC /*KHz*/)
{
    ang_tick_s = 4000.0f / FOSC;
}

void delay_us(word t)
{
    dword c, i;
    c = (float)t/(36.5 * ang_tick_s);
    for(i = 0; i < c; i++);
}

void delay_ms(word t)
{
    dword c, i, j = 0;
    c = (dword)(((float)t/(36.5 * ang_tick_s)) * 1000);  
    for(i = 0; i < c; i++);  
}

void delay_s(word t)
{
   delay_ms(t * 1000); 
}

