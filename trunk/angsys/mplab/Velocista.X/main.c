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

#include "main.h"


adc_t adc;
bool_t read_done;
dword last_time;

digital_pin_t F0, F1, B0, B1;
pwm_t pwm;
pwm_pin_t pwm0, pwm1;

void adc_conversion_completed_event(analog_id_t, word);

void setup(void)
{  
    ang_set_fosc(FOSC);
    tick_count_start();
    
    adc_initialize(&adc, ADC_CONFIG1_AN0TOAN3, ADC_DEFCONFIG2);
    
    pwm_initialize(FPWM, &pwm);
    
    pwm.open_channel(PINC2, &pwm0);
    pwm.open_channel(PINB3, &pwm1);
     
    read_done = false;
    adc.set_completed_event(&adc_conversion_completed_event);
    adc.read_async(A0);
  
    ang_create_digital_pin(PINC0, &F0);
    ang_create_digital_pin(PINB2, &F1);
    ang_create_digital_pin(PINC1, &B0);
    ang_create_digital_pin(PINB1, &B1);
    
    
    pwm0.write(100);
    pwm1.write(100);
    
    F0.set_mode(OUTPUT);
    B0.set_mode(OUTPUT);
    F1.set_mode(OUTPUT);
    B1.set_mode(OUTPUT);
    
    F0.write(LOW);
    B0.write(LOW);
    
    F1.write(LOW);
    B1.write(LOW);
    
    last_time = get_tick_count();
}

void loop(void)
{
    dword time, delta;
    time = get_tick_count();
    if((time - last_time) >= 1000)
    {
        F0.write(!F0.read());  
        last_time = time;
    }
    delay_ms(100);
}


void adc_conversion_completed_event(analog_id_t id, word value)
{
    if(id == A0)
        read_done = true;
}

