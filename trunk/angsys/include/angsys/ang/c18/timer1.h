/* 
 * File:   timer0.h
 * Author: Jesus Rocha
 *
 * Created on December 11, 2018, 3:01 AM
 */

#ifndef __ANG_C18_TIMER1_H__
#define	__ANG_C18_TIMER1_H__

//TODO

#define TMR1_SRC_INTERN         0X00 //default
#define TMR1_SRC_EXTERN         0X02
#define TMR1_SYNC               0X00 //default
#define TMR1_NOSYNC             0X04


ANG_EXTERN void tmr1_initialize(byte config);
ANG_EXTERN void tmr1_start(void);
ANG_EXTERN void tmr1_stop(void);
ANG_EXTERN void tmr1_reset(void);
ANG_EXTERN void tmr1_set(word value);
ANG_EXTERN word tmr1_get(void);
ANG_EXTERN void tmr1_set_interrupt(bool enabled);
ANG_EXTERN bool tmr1_get_interrupt(void);

#endif//__ANG_C18_TIMER1_H__

