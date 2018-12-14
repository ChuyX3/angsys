#include <angsys.h>

ANG_EXTERN void setup(void);
ANG_EXTERN void loop(void);

lpfunc_t high_int_vect_s = NULL;
lpfunc_t low_int_vect_s = NULL;

void interrupt_high_service_routine(void);
void interrupt_low_service_routine(void);

#pragma code high_vector = 0x08
void high_interrupt_vector(void) { 
    _asm goto interrupt_high_service_routine _endasm
}

#pragma code low_vector = 0x18
void low_interrupt_vector(void) { 
    _asm goto interrupt_low_service_routine _endasm
}

void interrupt_initialize(bool priori, lpfunc_t high_int_vect, lpfunc_t low_int_vect)
{
    RCONbits.IPEN = priori;
    high_int_vect_s = high_int_vect;
    low_int_vect_s = low_int_vect;
}

void main(void)
{
    setup();
    while(true) loop();
}

#pragma interrupt interrupt_high_service_routine save=section("MATH_DATA")
void interrupt_high_service_routine(void)
{
    if(high_int_vect_s != NULL)
        high_int_vect_s();
    
}

#pragma interruptlow interrupt_low_service_routine
void interrupt_low_service_routine(void)
{
    if(low_int_vect_s != NULL)
        low_int_vect_s();
}






