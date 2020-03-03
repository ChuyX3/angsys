/* 
 * File:   timer0.h
 * Author: Jesus Rocha
 *
 * Created on December 11, 2018, 3:01 AM
 */

#ifndef __ANG_C18_TIMER0_H__
#define	__ANG_C18_TIMER0_H__


#define TMR0_PRESCALE1          0X08
#define TMR0_PRESCALE2          0X00
#define TMR0_PRESCALE4          0X01
#define TMR0_PRESCALE8          0X02
#define TMR0_PRESCALE16         0X03
#define TMR0_PRESCALE32         0X04
#define TMR0_PRESCALE64         0X05
#define TMR0_PRESCALE128        0X06
#define TMR0_PRESCALE256        0X07

#define TMR0_8BITS              0X40
#define TMR0_16BITS             0X00

#define TMR0_CLK_EXTERN         0X20
#define TMR0_CLK_INTERN         0X00

#define TMR0_FALLEDGE           0X10
#define TMR0_RISEEDGE           0X00


ANG_EXTERN void tmr0_initialize(byte config);
ANG_EXTERN void tmr0_start(void);
ANG_EXTERN void tmr0_stop(void);
ANG_EXTERN void tmr0_reset(void);
ANG_EXTERN void tmr0_set(word value);
ANG_EXTERN word tmr0_get(void);
ANG_EXTERN void tmr0_set_interrupt(bool enabled);
ANG_EXTERN bool tmr0_get_interrupt(void);

#endif	/* __ANG_C18_TIMER0_H__ */

