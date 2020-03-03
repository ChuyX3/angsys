/* 
 * File:   main.h
 * Author: Jesus Rocha
 *
 * Created on December 28, 2018, 5:28 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <angsys.h>

#define FOSC 48000 //48MHz
#define FPWM 12 //12KHz
#define TCLK 0.083333f //0.08333uS = 4/48MHz

ANG_EXTERN void low_interrupt(void);
ANG_EXTERN void high_interrupt(void);

ANG_EXTERN void setup(void);
ANG_EXTERN void loop(void);

ANG_EXTERN adc_t adc;
ANG_EXTERN bool_t read_done;


#endif//MAIN_H

