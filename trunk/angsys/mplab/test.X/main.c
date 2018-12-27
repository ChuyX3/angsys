/* 
 * File:   main.c
 * Author: Jesus Rocha
 *
 * Created on March 4, 2018, 5:44 PM
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
adc_t adc;
bool_t read_done;
keyboard_t keys;

void adc_conversion_completed_event(analog_id_t, word);

void setup(void)
{  
    //interrupt_initialize(true,&high_interrupt,&low_interrupt);
    delays_initialize(FOSC);
    
    adc_initialize(&adc, ADC_CONFIG1_AN0TOAN3, ADC_DEFCONFIG2);
    
    lcd_create(&lcd, PINA5, PINA4, PINA3, PINA2, PINA1, PINA0);
    lcd.init(16, 2);
    
    keyboard_create(&keys,PINB0,PINB1,PINB2,PINB3,PINB4,PINB5,PINB6,PINB7);
    
    read_done =  false;
    adc.set_completed_event(&adc_conversion_completed_event);
    adc.read_async(A0);
}

enum states
{
    NUM1,
    OP,
    NUM2,
    RESULT
};

void loop(void)
{
    if(read_done)
    {
        read_done = false;
        adc.read_async(A0);
        
        
        
    }
}

void adc_conversion_completed_event(analog_id_t id, word value)
{
    switch(id)
    {
        case A0:
            adc.read_async(A1);
            break;
        case A1:
            adc.read_async(A2);
            break;
        case A2:
            adc.read_async(A3);
            break;
        case A3:
            read_done = true;
            //adc.read_async(A0);
            break;
        default:break;
    }
}