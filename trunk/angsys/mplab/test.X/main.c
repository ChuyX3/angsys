/* 
 * File:   main.c
 * Author: Jesus Rocha
 *
 * Created on March 4, 2018, 5:44 PM
 */

#include <p18f2550.h>

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
#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF

#include "main.h"


dword tick_count;
dword count;

//void lcd_initialize(void);
//void lcd_entry_mode(volatile near bool_t id, volatile near bool_t scroll);
//void lcd_display_control(volatile near bool_t display, volatile near bool_t cursor, volatile near bool_t blink);
//void lcd_clear(void);
//void lcd_home(void);
//void lcd_put(volatile near byte value);

lcd_t lcd;

void setup(void)
{  
    //interrupt_initialize(true,&high_interrupt,&low_interrupt);
    delays_initialize(FOSC);
    
    adc_initialize(ADC_CONFIG1_ALL_DIGITAL, ADC_DEFCONFIG2);
    
    lcd_create(&lcd, PINA5, PINA4, PINA3, PINA2, PINA1, PINA0);
    lcd.init(16, 2);
}

void loop(void)
{
    lcd.gotoxy(0,0);
    lcd.write("Jesus Angel");
    
    delay_ms(1000);
}
