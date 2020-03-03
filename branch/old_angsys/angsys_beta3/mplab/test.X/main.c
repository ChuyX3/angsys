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
#pragma config CCP2MX   = OFF // ON = PORTC, OFF = PORTB
#pragma config STVREN   = ON
#pragma config LVP      = OFF

#include "main.h"


adc_t adc;
bool_t read_done;
dword last_time;

digital_output_t F0, F1, B0, B1;
pwm_t pwm;
pwm_pin_t pwm0, pwm1;

void adc_conversion_completed_event(analog_id_t, word);

void setup(void)
{  
    //interrupt_initialize(true,&high_interrupt,&low_interrupt);
    ang_set_fosc(FOSC);
   
    
    adc_initialize(&adc, ADC_CONFIG1_AN0TOAN3, ADC_DEFCONFIG2);
    
    pwm_initialize(FPWM, &pwm);
    
    pwm.open_channel(PINC2, &pwm0);
    pwm.open_channel(PINB3, &pwm1);
     
    read_done = false;
    adc.set_completed_event(&adc_conversion_completed_event);
    adc.read_async(A0);
  
    ang_create_digital_output(PINC0, &F0);
    ang_create_digital_output(PINB2, &F1);
    ang_create_digital_output(PINC1, &B0);
    ang_create_digital_output(PINB1, &B1);
    
    
   // delay_ms(5000);
    pwm0.write(40);
    pwm1.write(40);
    
    F0.write(LOW);
    B0.write(LOW);
    
    F1.write(LOW);
    B1.write(LOW);
}

void loop(void)
{
    dword time, delta;
    if(read_done)
    {
        read_done = false;
      
        
        
        
        
        adc.read_async(A0);
       
    }
}


void adc_conversion_completed_event(analog_id_t id, word value)
{
    if(id == A0)
        read_done = true;
}
