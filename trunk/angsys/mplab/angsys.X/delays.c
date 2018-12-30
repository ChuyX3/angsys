/* 
 * File:   pwm.c
 * Author: Jesus Rocha
 *
 * Created on March 5, 2018, 12:22 PM
 */

#include <angsys.h>

float ang_tick_s = 1; //uS
dword tick_count_s = 0;
bool_t tick_count_enabled_s = false;
word tmr0_preset_s;



void ang_set_fosc(float FOSC /*KHz*/)
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

void tick_count_start(void)
{
    tmr0_initialize(TMR0_PRESCALE256);
    tmr0_preset_s = 0;//0xffff - (word)(1000 / ang_tick_s);
    tick_count_enabled_s = true;
    tmr0_set_interrupt(true);
    tmr0_set(tmr0_preset_s);
    tmr0_start();
}

dword get_tick_count(void)
{
    dword count;
    dword tmr0h,tmr0l;
    count = (tick_count_s * 0X10000);
    tmr0h = ((dword)TMR0H * 0X100);
    tmr0l = TMR0L;
    count = (count + tmr0h + tmr0l);
    return count * (ang_tick_s * 256);
}