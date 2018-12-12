/* 
 * File:   adc.h
 * Author: Jesus Rocha
 *
 * Created on December 11, 2018, 2:51 AM
 */

#ifndef __ANG_C18_ADC_H__
#define	__ANG_C18_ADC_H__

#ifdef	__cplusplus
extern "C" {
#endif

//Analog config 1//
//Analog Line Selection
#define ADC_CONFIG1_ALL_DIGITAL			0X0F
#define ADC_CONFIG1_AN0TOAN0			0X0E
#define ADC_CONFIG1_AN0TOAN1			0X0D
#define ADC_CONFIG1_AN0TOAN2			0X0C
#define ADC_CONFIG1_AN0TOAN3			0X0B
#define ADC_CONFIG1_AN0TOAN4			0X0A
#define ADC_CONFIG1_AN0TOAN5			0X09
#define ADC_CONFIG1_AN0TOAN6			0X08
#define ADC_CONFIG1_AN0TOAN7			0X07
#define ADC_CONFIG1_AN0TOAN8			0X06
#define ADC_CONFIG1_AN0TOAN9			0X05
#define ADC_CONFIG1_AN0TOAN10			0X04
#define ADC_CONFIG1_AN0TOAN11			0X03
#define ADC_CONFIG1_AN0TOAN12			0X02
#define ADC_CONFIG1_ALL_ANALOG			ADC_CONFIG1_AN0TOAN12

//Analog reference Selection
#define ADC_CONFIG1_DEFREF				0X00
#define ADC_CONFIG1_VREFP				0X10
#define ADC_CONFIG1_VREFN				0X20

//Analog config 2//
//Conversion Clock Selection
#define ADC_CONFIG2_FOSC2				0X00
#define ADC_CONFIG2_FOSC4				0X04
#define ADC_CONFIG2_FOSC8				0X01
#define ADC_CONFIG2_FOSC16				0X05
#define ADC_CONFIG2_FOSC32				0X02
#define ADC_CONFIG2_FOSC64				0X06
#define ADC_CONFIG2_FRC					0X07

//Acquisition Time Selection
#define ADC_CONFIG2_TADX0				0x08
#define ADC_CONFIG2_TADX2				0x10
#define ADC_CONFIG2_TADX4				0x18
#define ADC_CONFIG2_TADX6				0x28
#define ADC_CONFIG2_TADX8				0x30
#define ADC_CONFIG2_TADX12				0x38
#define ADC_CONFIG2_TADX16				0x40
#define ADC_CONFIG2_TADX20				0x48

//Result Format Selection
#define	ADC_CONFIG2_JUSTLEFT			0X00
#define	ADC_CONFIG2_JUSTRIGTH			0X80

//Defaul configuration 2
#define ADC_DEFCONFIG2			0XCF 
//0XCF  (ADC_FRC | ADC_TADX20 | ADC_JUSTRIGTH)

//Analog Channels//
#define ADC_AN0					0X00
#define ADC_AN1					0X01
#define ADC_AN2					0X02
#define ADC_AN3					0X03
#define ADC_AN4					0X04
#define ADC_AN5					0X05
#define ADC_AN6					0X06
#define ADC_AN7					0X07
#define ADC_AN8					0X08
#define ADC_AN9					0X09
#define ADC_AN10				0X0A
#define ADC_AN11				0X0B
#define ADC_AN12				0X0C

void adc_initialize(byte config1, byte config2); 
word adc_read(byte channel);
void adc_start_conversion(byte channel);
bool adc_is_busy(void);
void adc_wait_for_conversion(void);
word adc_get_last_conversion(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __ANG_C18_ADC_H__ */

