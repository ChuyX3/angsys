/* 
 * File:   delays.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 3:41 AM
 */

#ifndef __ANG_C18_DELAYS_H__
#define	__ANG_C18_DELAYS_H__

ANG_EXTERN void delays_initialize(float FOSC /*KHz*/);
ANG_EXTERN void delay_us(word t /*uS*/);
ANG_EXTERN void delay_ms(word t /*mS*/);
ANG_EXTERN void delay_s(word t /*S*/);

ANG_EXTERN void tick_count_start(void); //need timer 0
dword get_tick_count(void); //1 tick = 100uS

#endif//__ANG_C18_DELAYS_H__

