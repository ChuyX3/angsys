/* 
 * File:   main.c
 * Author: Jesus Rocha
 *
 * Created on December 28, 2018, 5:28 PM
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

#include "main.h"

dword last_time;
dword count = 0;

rom const char sin_table[] = { 
    0X80,0X83,0X86,0X89,0X8c,0X90,0X93,0X96,0X99,0X9c,
    0X9f,0Xa2,0Xa5,0Xa8,0Xab,0Xae,0Xb1,0Xb4,0Xb7,0Xb9,
    0Xbc,0Xbf,0Xc2,0Xc4,0Xc7,0Xc9,0Xcc,0Xce,0Xd1,0Xd3,
    0Xd6,0Xd8,0Xda,0Xdc,0Xde,0Xe1,0Xe3,0Xe5,0Xe6,0Xe8,
    0Xea,0Xec,0Xed,0Xef,0Xf0,0Xf2,0Xf3,0Xf5,0Xf6,0Xf7,
    0Xf8,0Xf9,0Xfa,0Xfb,0Xfc,0Xfc,0Xfd,0Xfe,0Xfe,0Xff,
    0Xff,0Xff,0Xff,0Xff,0Xff,0Xff,0Xff,0Xff,0Xff,0Xff,
    0Xfe,0Xfe,0Xfd,0Xfc,0Xfc,0Xfb,0Xfa,0Xf9,0Xf8,0Xf7,
    0Xf6,0Xf5,0Xf3,0Xf2,0Xf0,0Xef,0Xed,0Xec,0Xea,0Xe8,
    0Xe6,0Xe5,0Xe3,0Xe1,0Xde,0Xdc,0Xda,0Xd8,0Xd6,0Xd3,
    0Xd1,0Xce,0Xcc,0Xc9,0Xc7,0Xc4,0Xc2,0Xbf,0Xbc,0Xb9,
    0Xb7,0Xb4,0Xb1,0Xae,0Xab,0Xa8,0Xa5,0Xa2,0X9f,0X9c,
    0X99,0X96,0X93,0X90,0X8c,0X89,0X86,0X83,0X80,0X7d,
    0X7a,0X77,0X74,0X70,0X6d,0X6a,0X67,0X64,0X61,0X5e,
    0X5b,0X58,0X55,0X52,0X4f,0X4c,0X49,0X47,0X44,0X41,
    0X3e,0X3c,0X39,0X37,0X34,0X32,0X2f,0X2d,0X2a,0X28,
    0X26,0X24,0X22,0X1f,0X1d,0X1b,0X1a,0X18,0X16,0X14,
    0X13,0X11,0X10,0X0e,0X0d,0X0b,0X0a,0X09,0X08,0X07,
    0X06,0X05,0X04,0X04,0X03,0X02,0X02,0X01,0X01,0X01,
    0X01,0X01,0X00,0X01,0X01,0X01,0X01,0X01,0X02,0X02,
    0X03,0X04,0X04,0X05,0X06,0X07,0X08,0X09,0X0a,0X0b,
    0X0d,0X0e,0X10,0X11,0X13,0X14,0X16,0X18,0X1a,0X1b,
    0X1d,0X1f,0X22,0X24,0X26,0X28,0X2a,0X2d,0X2f,0X32,
    0X34,0X37,0X39,0X3c,0X3e,0X41,0X44,0X47,0X49,0X4c,
    0X4f,0X52,0X55,0X58,0X5b,0X5e,0X61,0X64,0X67,0X6a,
    0X6d,0X70,0X74,0X77,0X7a,0X7d
};


void setup(void)
{  
    ang_set_fosc(FOSC);
    tmr0_initialize(TMR0_8BITS | TMR0_CLK_EXTERN);
    TRISB = 0;
    TRISAbits.RA4 = 1;
    tmr0_start();
}

void loop(void)
{ 
    PORTB = sin_table[tmr0_get()];
}

