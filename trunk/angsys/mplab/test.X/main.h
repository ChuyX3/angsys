/* 
 * File:   main.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 10:25 PM
 */

#ifndef __MAIN_H__
#define	__MAIN_H__

#include <angsys.h>

#define FOSC 48000 //48MHz
#define FPWM 50 //12KHz
#define TCLK 0.083333f //0.08333uS = 4/48MHz

ANG_EXTERN void low_interrupt(void);
ANG_EXTERN void high_interrupt(void);

ANG_EXTERN void setup(void);
ANG_EXTERN void loop(void);

#endif//__MAIN_H__

