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


lcd_t lcd;
adc_t adc;
bool_t read_done;

void adc_conversion_completed_event(analog_id_t, word);

void setup(void)
{  
    //interrupt_initialize(true,&high_interrupt,&low_interrupt);
    delays_initialize(FOSC);
    
    adc_initialize(&adc, ADC_CONFIG1_AN0TOAN3, ADC_DEFCONFIG2);
    
    lcd_create(&lcd, PINB5, PINB4, PINB3, PINB2, PINB1, PINB0);
    lcd.init(16, 2);
    
    read_done =  false;
    adc.set_completed_event(&adc_conversion_completed_event);
    adc.read_async(A0);
}

void loop(void)
{
    if(read_done)
    {
        read_done = false;
        adc.read_async(A0);
        
        lcd.gotoxy(0,0);
        lcd.print("%d",adc.get_value(A0));
        lcd.gotoxy(8,0);
        lcd.print("%d",adc.get_value(A1));
        lcd.gotoxy(0,1);
        lcd.print("%d",adc.get_value(A2));
        lcd.gotoxy(8,1);
        lcd.print("%d",adc.get_value(A3));
    }
    delay_ms(100);
}

void adc_conversion_completed_event(analog_id_t id, word value)
{
    lcd.write("done");
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
            break;
        default:break;
    }
}