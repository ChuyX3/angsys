/* 
 * File:   digital_ports.h
 * Author: Jesus Rocha
 *
 * Created on December 13, 2018, 7:05 AM
 */

#ifndef __ANG_C18_DIGITAL_PORTS_H__
#define	__ANG_C18_DIGITAL_PORTS_H__


typedef enum port_name_tag
{
    portA = 0,
    portB = 1,
    portC = 2,
    portD = 3,
    portE = 4,
}port_name_t;

typedef enum pin_index_tag
{
    pin0 = 0,
    pin1 = 1,
    pin2 = 2,
    pin3 = 3,
    pin4 = 4,
    pin5 = 5,
    pin6 = 6,
    pin7 = 7,
}pin_index_t;

typedef enum pin_mode_tag
{
    OUTPUT = 0,
    INPUT = 1,
}pin_mode_t;


typedef enum pin_id_tag
{
    PINA0 = 0X00,
    PINA1 = 0X10,
    PINA2 = 0X20,
    PINA3 = 0X30,
    PINA4 = 0X40,
    PINA5 = 0X50,
    
    PINB0 = 0X01,
    PINB1 = 0X11,
    PINB2 = 0X21,
    PINB3 = 0X31,
    PINB4 = 0X41,
    PINB5 = 0X51,
    PINB6 = 0X61,
    PINB7 = 0X71,
    
    PINC0 = 0X02,
    PINC1 = 0X12,
    PINC2 = 0X22,
    //PINC3 = 0X32,
    //PINC4 = 0X42,
    //PINC5 = 0X52,
    PINC6 = 0X62,
    PINC7 = 0X72,
    
    PIND0 = 0X03,
    PIND1 = 0X13,
    PIND2 = 0X23,
    PIND3 = 0X33,
    PIND4 = 0X43,
    PIND5 = 0X53,
    PIND6 = 0X63,
    PIND7 = 0X73,
    
    PINE0 = 0X04,
    PINE1 = 0X14,
    PINE2 = 0X24,
}pin_id_t;


typedef struct digital_pin_tag digital_pin_t;
typedef struct digital_input_tag digital_input_t;
typedef struct digital_output_tag digital_output_t;

typedef rom far pin_id_t (*pin_get_id_t)(void);
typedef rom far level(*digital_input_read_t)(void);
typedef rom far void(*digital_output_write_t)(level);
typedef rom far pin_mode_t(*digital_pin_get_mode_t)(void);
typedef rom far void(*digital_pin_set_mode_t)(pin_mode_t);

struct digital_input_tag
{
    pin_get_id_t get_id;
    digital_input_read_t read;
    
};

struct digital_output_tag
{
    pin_get_id_t get_id;
    digital_output_write_t write;
};

struct digital_pin_tag
{
    pin_get_id_t get_id;
    digital_input_read_t read;
    digital_output_write_t write;
    digital_pin_get_mode_t get_mode;
    digital_pin_set_mode_t set_mode;
};

ANG_EXTERN pin_mode_t ang_get_pin_info(pin_id_t);
ANG_EXTERN void ang_create_digital_pin(pin_id_t id, digital_pin_t* pin);
ANG_EXTERN void ang_create_digital_input(pin_id_t id, digital_input_t* pin);
ANG_EXTERN void ang_create_digital_output(pin_id_t id, digital_output_t* pin);



#endif//__ANG_C18_DIGITAL_PORTS_H__

