/* 
 * File:   adc.c
 * Author: Jesus Rocha
 *
 * Created on March 5, 2018, 12:21 PM
 */

#include <angsys.h>

void adc_initialize(byte config1, byte config2)
{
  	ADCON1 = config1;
	ADCON2 = config2;  
}

word adc_read(byte channel)
{
	ADCON0=channel*4;
    ADCON0bits.ADON = HIGH;
    ADCON0bits.NOT_DONE = HIGH;
    while(ADCON0bits.NOT_DONE);
	return ((word)ADRESH << 8) | ADRESL;
}

void adc_start_conversion(byte channel)
{
	ADCON0=channel*4;
    ADCON0bits.ADON = HIGH;
    ADCON0bits.NOT_DONE = HIGH;
}

bool adc_is_busy(void)
{
    return ADCON0bits.NOT_DONE;
}

void adc_wait_for_conversion(void)
{
    while(ADCON0bits.NOT_DONE);
}

word adc_get_last_conversion(void)
{
    return ((word)ADRESH << 8) | ADRESL;
}

