/* 
 * File:   adc.c
 * Author: Jesus Rocha
 *
 * Created on March 5, 2018, 12:21 PM
 */

#include <angsys.h>

typedef struct adc_handle_tag
{
    short buffer[ADC_COUNT];
    adc_conversion_completed_event_t callback;
}adc_handle_t;

adc_handle_t adc_handle;

bool_t adc_is_busy(void);
short adc_get_value(analog_id_t id);
short adc_read(analog_id_t channel);
bool_t adc_read_async(analog_id_t channel);
bool_t adc_interrupt_callback(void);
void adc_set_conversion_completed_event(adc_conversion_completed_event_t);

void adc_initialize(adc_t* adc, adc_config1_t config1, adc_config2_t config2)
{
    analog_id_t i;
  	ADCON1 = config1;
	ADCON2 = config2; 
    
    adc_handle.callback = NULL;
    for(i = A0; i < ADC_COUNT; i++)
        adc_handle.buffer[i] = 0;
    
    adc->get_value = (adc_get_t)&adc_get_value;
    adc->is_busy =(adc_is_busy_t)&adc_is_busy;
    adc->read = (adc_get_t)&adc_read;
    adc->read_async = (adc_read_async_t)&adc_read_async;
    adc->set_completed_event = (adc_conversion_completed_t)&adc_set_conversion_completed_event;
}

bool_t adc_is_busy(void)
{
    return ADCON0bits.NOT_DONE;
}

short adc_get_value(analog_id_t id)
{
    return adc_handle.buffer[id];
}

short adc_read(analog_id_t channel)
{
	ADCON0bits.CHS = channel;
    ADCON0bits.ADON = HIGH;
    ADCON0bits.NOT_DONE = HIGH;
    while(ADCON0bits.NOT_DONE);
	adc_handle.buffer[channel] = ((short)ADRESH << 8) | ADRESL;
    return adc_handle.buffer[channel];
}

bool_t adc_read_async(analog_id_t channel)
{
    if(ADCON0bits.NOT_DONE == true)
        return false;
	ADCON0bits.CHS = channel;
    ADCON0bits.ADON = HIGH;
    ADCON0bits.NOT_DONE = HIGH;
    return true;
}

void adc_set_conversion_completed_event(adc_conversion_completed_event_t callback)
{
    adc_handle.callback = callback;
}

bool_t adc_interrupt_callback(void)
{
    if(adc_handle.callback == NULL)
        return false;
    adc_handle.buffer[ADCON0bits.CHS] = ((short)ADRESH << 8) | ADRESL;
    adc_handle.callback(ADCON0bits.CHS, adc_handle.buffer[ADCON0bits.CHS]);
    return true;
}