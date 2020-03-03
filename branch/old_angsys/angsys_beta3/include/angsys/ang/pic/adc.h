/* 
 * File:   adc.h
 * Author: Jesus Rocha
 *
 * Created on December 11, 2018, 2:51 AM
 */

#ifndef __ANG_C18_ADC_H__
#define	__ANG_C18_ADC_H__


#ifdef __cplusplus



#else//C

//Analog config 1//
//Analog Line Selection
typedef enum
{
    ADC_CONFIG1_DEFREF = 0X00,
    ADC_CONFIG1_VREFP = 0X10,
    ADC_CONFIG1_VREFN = 0X20,

    ADC_CONFIG1_ALL_DIGITAL = 0X0F,
    ADC_CONFIG1_AN0TOAN0 = 0X0E,
    ADC_CONFIG1_AN0TOAN1 = 0X0D,
    ADC_CONFIG1_AN0TOAN2 = 0X0C,
    ADC_CONFIG1_AN0TOAN3 = 0X0B,
    ADC_CONFIG1_AN0TOAN4 = 0X0A,
    ADC_CONFIG1_AN0TOAN5 = 0X09,
    ADC_CONFIG1_AN0TOAN6 = 0X08,
    ADC_CONFIG1_AN0TOAN7 = 0X07,
    ADC_CONFIG1_AN0TOAN8 = 0X06,
    ADC_CONFIG1_AN0TOAN9 = 0X05,
    ADC_CONFIG1_AN0TOAN10 = 0X04,
    ADC_CONFIG1_AN0TOAN11 = 0X03,
    ADC_CONFIG1_AN0TOAN12 = 0X02,
    ADC_CONFIG1_ALL_ANALOG = ADC_CONFIG1_AN0TOAN12  
}adc_config1_t;

//Analog config 2//
typedef enum
{
    ADC_CONFIG2_FOSC2 = 0X00,
    ADC_CONFIG2_FOSC4 = 0X04,
    ADC_CONFIG2_FOSC8 = 0X01,
    ADC_CONFIG2_FOSC16 = 0X05,
    ADC_CONFIG2_FOSC32 = 0X02,
    ADC_CONFIG2_FOSC64 = 0X06,
    ADC_CONFIG2_FRC = 0X07,
          
    ADC_CONFIG2_TADX0 = 0x08,
    ADC_CONFIG2_TADX2 = 0x10,
    ADC_CONFIG2_TADX4 = 0x18,
    ADC_CONFIG2_TADX6 = 0x28,
    ADC_CONFIG2_TADX8 = 0x30,
    ADC_CONFIG2_TADX12 = 0x38,
    ADC_CONFIG2_TADX16 = 0x40,
    ADC_CONFIG2_TADX20 = 0x48,
            
    ADC_CONFIG2_JUSTLEFT = 0X00,
    ADC_CONFIG2_JUSTRIGTH = 0X80,
    ADC_DEFCONFIG2 = 0XCF
}adc_config2_t;


//Analog Channels//
typedef enum analog_id_tag
{   
    A0 = 0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,
    A11,
    A12,
    ADC_COUNT
}analog_id_t;

typedef struct adc_tag adc_t;

typedef rom far void (*adc_conversion_completed_event_t)(analog_id_t, word);
typedef rom far void (*adc_conversion_completed_t)(adc_conversion_completed_event_t);

typedef rom far word (*adc_get_t)(analog_id_t);
typedef rom far bool (*adc_is_busy_t)(void);
typedef rom far bool (*adc_read_async_t)(analog_id_t);

struct adc_tag
{
    adc_is_busy_t is_busy;
    adc_get_t read;
    adc_get_t get_value;
    adc_read_async_t read_async;
    adc_conversion_completed_t set_completed_event;
};

ANG_EXTERN void adc_initialize(adc_t* adc, adc_config1_t config1, adc_config2_t config2); 

#endif//__cplusplus
#endif//__ANG_C18_ADC_H__

