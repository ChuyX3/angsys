/* 
 * File:   timer2.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 3:55 AM
 */

#ifndef __ANG_C18_TIMER2_H__
#define	__ANG_C18_TIMER2_H__


#define TMR2_PRESCALE1          0X00
#define TMR2_PRESCALE4          0X01
#define TMR2_PRESCALE16         0X03 //or 0X02

#define TMR2_POSTSCALE(VALUE)    (0X78&((VALUE) << 3)) // 1 : (VALUE + 1)



void tmr2_initialize(byte config);
void tmr2_start(void);
void tmr2_stop(void);
void tmr2_reset(void);
void tmr2_set(byte value);
byte tmr2_get(void);
void tmr2_period_set(byte value);
byte tmr2_period_get(void);


#endif//__ANG_C18_TIMER2_H__

