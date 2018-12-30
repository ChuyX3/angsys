/* 
 * File:   pwm.h
 * Author: Jesus Rocha
 *
 * Created on December 28, 2018, 6:06 AM
 */

#ifndef __ANG_PIC_PWM_H__
#define	__ANG_PIC_PWM_H__

typedef struct pwm_tag pwm_t;
typedef struct pwm_pin_tag pwm_pin_t;

typedef rom far void (*pwm_enable_t)(bool_t);
typedef rom far bool (*pwm_is_enabled_t)(void);
typedef rom far void (*pwm_write_t)(float);
typedef rom far void (*pwm_open_channel_t)(pin_id_t, pwm_pin_t*);
typedef rom far void (*pwm_set_frequency_t)(dword);

struct pwm_pin_tag
{
    pwm_write_t write;
};

struct pwm_tag
{
    pwm_enable_t enable;
    pwm_is_enabled_t is_enabled; 
    pwm_open_channel_t open_channel;
    pwm_set_frequency_t set_frequency;
};

void pwm_initialize(dword FPWM /*KHz*/, pwm_t* pwm);

#endif//__ANG_PIC_PWM_H__
