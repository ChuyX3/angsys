/* 
 * File:   main.c
 * Author: Jesus Rocha
 *
 * Created on 1 de julio de 2019, 03:31 AM
 */

#pragma config FOSC 	= HSPLL_HS 
#pragma config PLLDIV	= 1
#pragma config CPUDIV 	= OSC1_PLL2
#pragma config USBDIV   = 1
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = ON
#pragma config BORV     = 3
#pragma config VREGEN   = ON
#pragma config WDT      = OFF
#pragma config MCLRE    = ON
#pragma config PBADEN   = OFF
#pragma config CCP2MX   = OFF // ON = PORTC, OFF = PORTB
#pragma config STVREN   = ON
#pragma config LVP      = OFF

#include <p18f2550.h>

#include <angsys.h>

#define FOSC 48000 //48MHz
#define FPWM 12 //12KHz
#define TCLK 0.083333f //0.08333uS = 4/48MHz

ANG_EXTERN void low_interrupt(void);
ANG_EXTERN void high_interrupt(void);

ANG_EXTERN void setup(void);
ANG_EXTERN void loop(void);

void setup(void)
{


    
}

void loop(void)
{


    
}
