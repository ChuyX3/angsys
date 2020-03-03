#include <angsys.h>

ANG_EXTERN void setup(void);
ANG_EXTERN void loop(void);
ANG_EXTERN bool_t adc_interrupt_callback(void);

lpfunc_t high_int_vect_s;
lpfunc_t low_int_vect_s;

ANG_EXTERN dword tick_count_s;
ANG_EXTERN bool_t tick_count_enabled_s;
ANG_EXTERN word tmr0_preset_s;

void interrupt_high_service_routine(void);
void interrupt_low_service_routine(void);

#pragma code high_vector = 0x08
void high_interrupt_vector(void) { 
#ifdef __18CXX
    _asm goto interrupt_high_service_routine _endasm
#elif defined __XC8
    asm( "goto interrupt_high_service_routine");
#endif
}

#pragma code low_vector = 0x18
void low_interrupt_vector(void) { 
#ifdef __18CXX
    _asm goto interrupt_low_service_routine _endasm
#elif defined __XC8
    asm( "goto interrupt_low_service_routine");
#endif
}

void main(void)
{
    high_int_vect_s = NULL;
    low_int_vect_s = NULL;
    ADCON1 = ADC_CONFIG1_ALL_DIGITAL;
    RCONbits.IPEN = true;
    
    INTCONbits.GIE = true;
    
    setup();
    while(true) loop();
}

#pragma interrupt interrupt_high_service_routine save=section("MATH_DATA")
void interrupt_high_service_routine(void)
{ 
    if(PIR1bits.ADIF == true)
    {
        PIR1bits.ADIF = false;
        if(adc_interrupt_callback())
            return;
    }  
    
    if(INTCONbits.T0IF == true && tick_count_enabled_s)
    {
        INTCONbits.T0IF = false;
        tmr0_set(tmr0_preset_s);
        tick_count_s ++;
        return;
    }
    
    if(high_int_vect_s != NULL)
        high_int_vect_s();
    
}

#pragma interruptlow interrupt_low_service_routine
void interrupt_low_service_routine(void)
{
    if(PIR1bits.ADIF == true)
    {
        PIR1bits.ADIF = false;
        if(adc_interrupt_callback())
            return;
    }

    if(INTCONbits.T0IF == true && tick_count_enabled_s)
    {
        INTCONbits.T0IF = false;
        tmr0_set(tmr0_preset_s);
        tick_count_s ++;
        return;
    }   
    
    if(low_int_vect_s != NULL)
        low_int_vect_s();
}






