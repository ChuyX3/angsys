#include <angsys.h>

extern float ang_tick_s;

void pwm_enable(bool_t);
bool_t pwm_is_enabled(void);

void pwm_write_ch1(float);
void pwm_write_ch2(float);

void pwm_open_channel(pin_id_t, pwm_pin_t*);
void pwm_set_frequency(dword);


void pwm_initialize(dword FPWM, pwm_t* pwm)
{
    int32 pr2;
    dword F;
	CCP1CON = 0X0C;
	CCP2CON = 0X0C;
    F = 1000.0f / ang_tick_s;
    pr2 = F / (FPWM);
    
    if(pr2 > 1024)
    {
        T2CON = TMR2_PRESCALE16;
        PR2 = pr2 / 16 - 1;
    }
    else if(pr2 > 256)
    {
        T2CON = TMR2_PRESCALE4;
        PR2 = pr2 / 4 - 1; 
    }
    else
    {
        T2CON = TMR2_PRESCALE1;
        PR2 = pr2 - 1; 
    }
    
    pwm_write_ch1(0);
    pwm_write_ch2(0);
    
    pwm->enable = (pwm_enable_t)&pwm_enable;
    pwm->is_enabled = (pwm_is_enabled_t)&pwm_is_enabled;
    pwm->open_channel = (pwm_open_channel_t) &pwm_open_channel;
    pwm->set_frequency = (pwm_set_frequency_t)&pwm_set_frequency;
    
    T2CONbits.TMR2ON = true;
}

void pwm_enable(bool_t value)
{
    T2CONbits.TMR2ON = value;
}

bool_t pwm_is_enabled(void)
{
    return T2CONbits.TMR2ON;
}

void pwm_write_ch1(float dutyCycle)
{
    int32 pwm = 4.0f * (PR2 + 1) * dutyCycle / 100.0f ;
    CCPR1L = pwm >> 2;
    CCP1CONbits.DC1B0 = pwm & 0X1? true : false;
    CCP1CONbits.DC1B1 = pwm & 0X2? true : false;
}

void pwm_write_ch2(float dutyCycle)
{
    int32 pwm = 4.0f * (PR2 + 1) * dutyCycle / 100.0f ;
    CCPR2L = pwm >> 2;
    CCP2CONbits.DC2B0 = pwm & 0X1? true : false;
    CCP2CONbits.DC2B1 = pwm & 0X2? true : false; 
}

void pwm_open_channel(pin_id_t id, pwm_pin_t* out)
{
    switch(id)
    {
        case PINC2:
            TRISCbits.RC2 = OUTPUT;
            out->write = (pwm_write_t)&pwm_write_ch1;
            break;
        case PINC1:
            TRISCbits.RC1 = OUTPUT;
            out->write = (pwm_write_t)&pwm_write_ch2;
            break;
        case PINB3:
            TRISBbits.RB3 = OUTPUT;
            out->write = (pwm_write_t)&pwm_write_ch2;
            break;
        default:break;
    }
}

void pwm_set_frequency(dword FPWM)
{
    int32 pr2;
    dword F;
    F = 1000.0f / ang_tick_s;
    pr2 = F / (FPWM);
    
    if(pr2 > 1024)
    {
        T2CON = TMR2_PRESCALE16;
        PR2 = pr2 / 16 - 1;
    }
    else if(pr2 > 256)
    {
        T2CON = TMR2_PRESCALE4;
        PR2 = pr2 / 4 - 1; 
    }
    else
    {
        T2CON = TMR2_PRESCALE1;
        PR2 = pr2 - 1; 
    }
}