#include <angsys.h>


level get_PINA0(void){ return PORTAbits.RA0; }
level get_PINA1(void){ return PORTAbits.RA1; }
level get_PINA2(void){ return PORTAbits.RA2; }
level get_PINA3(void){ return PORTAbits.RA3; }
level get_PINA4(void){ return PORTAbits.RA4; }
level get_PINA5(void){ return PORTAbits.RA5; }

level get_PINB0(void){ return PORTBbits.RB0; }
level get_PINB1(void){ return PORTBbits.RB1; }
level get_PINB2(void){ return PORTBbits.RB2; }
level get_PINB3(void){ return PORTBbits.RB3; }
level get_PINB4(void){ return PORTBbits.RB4; }
level get_PINB5(void){ return PORTBbits.RB5; }
level get_PINB6(void){ return PORTBbits.RB6; }
level get_PINB7(void){ return PORTBbits.RB7; }

level get_PINC0(void){ return PORTCbits.RC0; }
level get_PINC1(void){ return PORTCbits.RC1; }
level get_PINC2(void){ return PORTCbits.RC2; }
//level get_PINC3(void){ return PORTCbits.RC3; }
//level get_PINC4(void){ return PORTCbits.RC4; }
//level get_PINC5(void){ return PORTCbits.RC5; }
level get_PINC6(void){ return PORTCbits.RC6; }
level get_PINC7(void){ return PORTCbits.RC7; }


pin_id_t get_PINA0_id(void){ return PINA0; }
pin_id_t get_PINA1_id(void){ return PINA1; }
pin_id_t get_PINA2_id(void){ return PINA2; }
pin_id_t get_PINA3_id(void){ return PINA3; }
pin_id_t get_PINA4_id(void){ return PINA4; }
pin_id_t get_PINA5_id(void){ return PINA5; }

pin_id_t get_PINB0_id(void){ return PINB0; }
pin_id_t get_PINB1_id(void){ return PINB1; }
pin_id_t get_PINB2_id(void){ return PINB2; }
pin_id_t get_PINB3_id(void){ return PINB3; }
pin_id_t get_PINB4_id(void){ return PINB4; }
pin_id_t get_PINB5_id(void){ return PINB5; }
pin_id_t get_PINB6_id(void){ return PINB6; }
pin_id_t get_PINB7_id(void){ return PINB7; }

pin_id_t get_PINC0_id(void){ return PINC0; }
pin_id_t get_PINC1_id(void){ return PINC1; }
pin_id_t get_PINC2_id(void){ return PINC2; }
//pin_id_t get_PINC3_id(void){ return PORTCbits.RC3; }
//pin_id_t get_PINC4_id(void){ return PORTCbits.RC4; }
//pin_id_t get_PINC5_id(void){ return PORTCbits.RC5; }
pin_id_t get_PINC6_id(void){ return PINC6; }
pin_id_t get_PINC7_id(void){ return PINC7; }


void set_PINA0(level value){ PORTAbits.RA0 = value; }
void set_PINA1(level value){ PORTAbits.RA1 = value; }
void set_PINA2(level value){ PORTAbits.RA2 = value; }
void set_PINA3(level value){ PORTAbits.RA3 = value; }
void set_PINA4(level value){ PORTAbits.RA4 = value; }
void set_PINA5(level value){ PORTAbits.RA5 = value; }

void set_PINB0(level value){ PORTBbits.RB0 = value; }
void set_PINB1(level value){ PORTBbits.RB1 = value; }
void set_PINB2(level value){ PORTBbits.RB2 = value; }
void set_PINB3(level value){ PORTBbits.RB3 = value; }
void set_PINB4(level value){ PORTBbits.RB4 = value; }
void set_PINB5(level value){ PORTBbits.RB5 = value; }
void set_PINB6(level value){ PORTBbits.RB6 = value; }
void set_PINB7(level value){ PORTBbits.RB7 = value; }

void set_PINC0(level value){ PORTCbits.RC0 = value; }
void set_PINC1(level value){ PORTCbits.RC1 = value; }
void set_PINC2(level value){ PORTCbits.RC2 = value; }
//v set_PINC3(level value){ PORTCbits.RC3 = value; }
//void set_PINC4(level value){ PORTCbits.RC4 = value; }
//void set_PINC5(level value){ PORTCbits.RC5 = value; }
void set_PINC6(level value){ PORTCbits.RC6 = value; }
void set_PINC7(level value){ PORTCbits.RC7 = value; }




pin_mode_t get_PINA0_mode(void){ return TRISAbits.RA0; }
pin_mode_t get_PINA1_mode(void){ return TRISAbits.RA1; }
pin_mode_t get_PINA2_mode(void){ return TRISAbits.RA2; }
pin_mode_t get_PINA3_mode(void){ return TRISAbits.RA3; }
pin_mode_t get_PINA4_mode(void){ return TRISAbits.RA4; }
pin_mode_t get_PINA5_mode(void){ return TRISAbits.RA5; }

pin_mode_t get_PINB0_mode(void){ return TRISBbits.RB0; }
pin_mode_t get_PINB1_mode(void){ return TRISBbits.RB1; }
pin_mode_t get_PINB2_mode(void){ return TRISBbits.RB2; }
pin_mode_t get_PINB3_mode(void){ return TRISBbits.RB3; }
pin_mode_t get_PINB4_mode(void){ return TRISBbits.RB4; }
pin_mode_t get_PINB5_mode(void){ return TRISBbits.RB5; }
pin_mode_t get_PINB6_mode(void){ return TRISBbits.RB6; }
pin_mode_t get_PINB7_mode(void){ return TRISBbits.RB7; }

pin_mode_t get_PINC0_mode(void){ return TRISCbits.RC0; }
pin_mode_t get_PINC1_mode(void){ return TRISCbits.RC1; }
pin_mode_t get_PINC2_mode(void){ return TRISCbits.RC2; }
//pin_mode_t get_PINC3(void){ return TRISCbits.RC3; }
//pin_mode_t get_PINC4(void){ return TRISCbits.RC4; }
//pin_mode_t get_PINC5(void){ return TRISCbits.RC5; }
pin_mode_t get_PINC6_mode(void){ return TRISCbits.RC6; }
pin_mode_t get_PINC7_mode(void){ return TRISCbits.RC7; }


void set_PINA0_mode(level value){ TRISAbits.RA0 = value; }
void set_PINA1_mode(level value){ TRISAbits.RA1 = value; }
void set_PINA2_mode(level value){ TRISAbits.RA2 = value; }
void set_PINA3_mode(level value){ TRISAbits.RA3 = value; }
void set_PINA4_mode(level value){ TRISAbits.RA4 = value; }
void set_PINA5_mode(level value){ TRISAbits.RA5 = value; }

void set_PINB0_mode(level value){ TRISBbits.RB0 = value; }
void set_PINB1_mode(level value){ TRISBbits.RB1 = value; }
void set_PINB2_mode(level value){ TRISBbits.RB2 = value; }
void set_PINB3_mode(level value){ TRISBbits.RB3 = value; }
void set_PINB4_mode(level value){ TRISBbits.RB4 = value; }
void set_PINB5_mode(level value){ TRISBbits.RB5 = value; }
void set_PINB6_mode(level value){ TRISBbits.RB6 = value; }
void set_PINB7_mode(level value){ TRISBbits.RB7 = value; }

void set_PINC0_mode(level value){ TRISCbits.RC0 = value; }
void set_PINC1_mode(level value){ TRISCbits.RC1 = value; }
void set_PINC2_mode(level value){ TRISCbits.RC2 = value; }
//v set_PINC3_mode(level value){ TRISCbits.RC3 = value; }
//void set_PINC4_mode(level value){ TRISCbits.RC4 = value; }
//void set_PINC5_mode(level value){ TRISCbits.RC5 = value; }
void set_PINC6_mode(level value){ TRISCbits.RC6 = value; }
void set_PINC7_mode(level value){ TRISCbits.RC7 = value; }




void ang_create_digital_pin(pin_id_t id, digital_pin_t* pin)
{
    switch(id)
    {
        case PINA0:
            pin->write = (digital_output_write_t)&set_PINA0;
            pin->read = (digital_input_read_t)&get_PINA0;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA0_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA0_mode;
            pin->get_id = (pin_get_id_t)&get_PINA0_id;
            break;
        case PINA1:
            pin->write = (digital_output_write_t)&set_PINA1;
            pin->read = (digital_input_read_t)&get_PINA1;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA1_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA1_mode;
            pin->get_id = (pin_get_id_t)&get_PINA1_id;
            break;
        case PINA2:
            pin->write = (digital_output_write_t)&set_PINA2;
            pin->read = (digital_input_read_t)&get_PINA2;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA2_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA2_mode;
            pin->get_id = (pin_get_id_t)&get_PINA2_id;
            break;
        case PINA3:
            pin->write = (digital_output_write_t)&set_PINA3;
            pin->read = (digital_input_read_t)&get_PINA3;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA3_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA3_mode;
            pin->get_id = (pin_get_id_t)&get_PINA3_id;
            break;
        case PINA4:
            pin->write = (digital_output_write_t)&set_PINA4;
            pin->read = (digital_input_read_t)&get_PINA4;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA4_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA4_mode;
            pin->get_id = (pin_get_id_t)&get_PINA4_id;
            break;
        case PINA5:
            pin->write = (digital_output_write_t)&set_PINA5;
            pin->read = (digital_input_read_t)&get_PINA5;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINA5_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINA5_mode;
            pin->get_id = (pin_get_id_t)&get_PINA5_id;
            break;
            
        case PINB0:
            pin->write = (digital_output_write_t)&set_PINB0;
            pin->read = (digital_input_read_t)&get_PINB0;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB0_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB0_mode;
            pin->get_id = (pin_get_id_t)&get_PINB0_id;
            break;
        case PINB1:
            pin->write = (digital_output_write_t)&set_PINB1;
            pin->read = (digital_input_read_t)&get_PINB1;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB1_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB1_mode;
            pin->get_id = (pin_get_id_t)&get_PINB1_id;
            break;
        case PINB2:
            pin->write = (digital_output_write_t)&set_PINB2;
            pin->read = (digital_input_read_t)&get_PINB2;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB2_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB2_mode;
            pin->get_id = (pin_get_id_t)&get_PINB2_id;
            break;
        case PINB3:
            pin->write = (digital_output_write_t)&set_PINB3;
            pin->read = (digital_input_read_t)&get_PINB3;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB3_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB3_mode;
            pin->get_id = (pin_get_id_t)&get_PINB3_id;
            break;
        case PINB4:
            pin->write = (digital_output_write_t)&set_PINB4;
            pin->read = (digital_input_read_t)&get_PINB4;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB4_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB4_mode;
            pin->get_id = (pin_get_id_t)&get_PINB4_id;
            break;
        case PINB5:
            pin->write = (digital_output_write_t)&set_PINB5;
            pin->read = (digital_input_read_t)&get_PINB5;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB5_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB5_mode;
            pin->get_id = (pin_get_id_t)&get_PINB5_id;
            break;
        case PINB6:
            pin->write = (digital_output_write_t)&set_PINB6;
            pin->read = (digital_input_read_t)&get_PINB6;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB6_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB6_mode;
            pin->get_id = (pin_get_id_t)&get_PINB6_id;
            break;
        case PINB7:
            pin->write = (digital_output_write_t)&set_PINB7;
            pin->read = (digital_input_read_t)&get_PINB7;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINB7_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINB7_mode;
            pin->get_id = (pin_get_id_t)&get_PINB7_id;
            break;
        
            
         case PINC0:
            pin->write = (digital_output_write_t)&set_PINC0;
            pin->read = (digital_input_read_t)&get_PINC0;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINC0_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINC0_mode;
            pin->get_id = (pin_get_id_t)&get_PINC0_id;
            break;
        case PINC1:
            pin->write = (digital_output_write_t)&set_PINC1;
            pin->read = (digital_input_read_t)&get_PINC1;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINC1_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINC1_mode;
            pin->get_id = (pin_get_id_t)&get_PINC1_id;
            break;
        case PINC2:
            pin->write = (digital_output_write_t)&set_PINC2;
            pin->read = (digital_input_read_t)&get_PINC2;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINC2_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINC2_mode;
            pin->get_id = (pin_get_id_t)&get_PINC2_id;
            break;  
        case PINC6:
            pin->write = (digital_output_write_t)&set_PINC6;
            pin->read = (digital_input_read_t)&get_PINC6;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINC6_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINC6_mode;
            pin->get_id = (pin_get_id_t)&get_PINC6_id;
            break;
        case PINC7:
            pin->write = (digital_output_write_t)&set_PINC7;
            pin->read = (digital_input_read_t)&get_PINC7;
            pin->set_mode = (digital_pin_set_mode_t)&set_PINC7_mode;
            pin->get_mode = (digital_pin_get_mode_t)&get_PINC7_mode;
            pin->get_id = (pin_get_id_t)&get_PINC7_id;
            break;
    }
}


void ang_create_digital_input(pin_id_t id, digital_input_t* pin)
{
    switch(id)
    {
        case PINA0:
            TRISAbits.RA0 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA0;
            pin->get_id = (pin_get_id_t)&get_PINA0_id;
            break;
        case PINA1:
            TRISAbits.RA1 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA1;
            pin->get_id = (pin_get_id_t)&get_PINA1_id;
            break;
        case PINA2:
            TRISAbits.RA2 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA2;
            pin->get_id = (pin_get_id_t)&get_PINA2_id;
            break;
        case PINA3:
            TRISAbits.RA3 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA3;
            pin->get_id = (pin_get_id_t)&get_PINA3_id;
            break;
        case PINA4:
            TRISAbits.RA4 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA4;
            pin->get_id = (pin_get_id_t)&get_PINA4_id;
            break;
        case PINA5:
            TRISAbits.RA5 = INPUT;
            pin->read = (digital_input_read_t)&get_PINA5;
            pin->get_id = (pin_get_id_t)&get_PINA5_id;
            break;
            
        case PINB0:
            TRISBbits.RB0 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB0;
            pin->get_id = (pin_get_id_t)&get_PINB0_id;
            break;
        case PINB1:
            TRISBbits.RB1 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB1;
            pin->get_id = (pin_get_id_t)&get_PINB1_id;
            break;
        case PINB2:
            TRISBbits.RB2 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB2;
            pin->get_id = (pin_get_id_t)&get_PINB2_id;
            break;
        case PINB3:
            TRISBbits.RB3 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB3;
            pin->get_id = (pin_get_id_t)&get_PINB3_id;
            break;
        case PINB4:
            TRISBbits.RB4 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB4;
            pin->get_id = (pin_get_id_t)&get_PINB4_id;
            break;
        case PINB5:
            TRISBbits.RB5 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB5;
            pin->get_id = (pin_get_id_t)&get_PINB5_id;
            break;
        case PINB6:
            TRISBbits.RB6 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB6;
            pin->get_id = (pin_get_id_t)&get_PINB6_id;
            break;
        case PINB7:
            TRISBbits.RB7 = INPUT;
            pin->read = (digital_input_read_t)&get_PINB7;
            pin->get_id = (pin_get_id_t)&get_PINB7_id;
            break;
        
            
         case PINC0:
            TRISCbits.RC0 = INPUT;
            pin->read = (digital_input_read_t)&get_PINC0;
            pin->get_id = (pin_get_id_t)&get_PINC0_id;
            break;
        case PINC1:
            TRISCbits.RC1 = INPUT;
            pin->read = (digital_input_read_t)&get_PINC1;
            pin->get_id = (pin_get_id_t)&get_PINC1_id;
            break;
        case PINC2:
            TRISCbits.RC2 = INPUT;
            pin->read = (digital_input_read_t)&get_PINC2;
            pin->get_id = (pin_get_id_t)&get_PINC2_id;
            break;  
        case PINC6:
            TRISCbits.RC6 = INPUT;
            pin->read = (digital_input_read_t)&get_PINC6;
            pin->get_id = (pin_get_id_t)&get_PINC6_id;
            break;
        case PINC7:
            TRISCbits.RC7 = INPUT;
            pin->read = (digital_input_read_t)&get_PINC7;
            pin->get_id = (pin_get_id_t)&get_PINC7_id;
            break;
    }
}

void ang_create_digital_output(pin_id_t id, digital_output_t* pin)
{
    switch(id)
    {
        case PINA0:
            TRISAbits.RA0 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA0;
            pin->get_id = (pin_get_id_t)&get_PINA0_id;
            break;
        case PINA1:
            TRISAbits.RA1 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA1;
            pin->get_id = (pin_get_id_t)&get_PINA1_id;
            break;
        case PINA2:
            TRISAbits.RA2 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA2;
            pin->get_id = (pin_get_id_t)&get_PINA2_id;
            break;
        case PINA3:
            TRISAbits.RA3 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA3;
            pin->get_id = (pin_get_id_t)&get_PINA3_id;
            break;
        case PINA4:
            TRISAbits.RA4 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA4;
            pin->get_id = (pin_get_id_t)&get_PINA4_id;
            break;
        case PINA5:
            TRISAbits.RA5 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINA5;
            pin->get_id = (pin_get_id_t)&get_PINA5_id;
            break;
            
        case PINB0:
            TRISBbits.RB0 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB0;
            pin->get_id = (pin_get_id_t)&get_PINB0_id;
            break;
        case PINB1:
            TRISBbits.RB1 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB1;
            pin->get_id = (pin_get_id_t)&get_PINB1_id;
            break;
        case PINB2:
            TRISBbits.RB2 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB2;
            pin->get_id = (pin_get_id_t)&get_PINB2_id;
            break;
        case PINB3:
            TRISBbits.RB3 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB3;
            pin->get_id = (pin_get_id_t)&get_PINB3_id;
            break;
        case PINB4:
            TRISBbits.RB4 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB4;
            pin->get_id = (pin_get_id_t)&get_PINB4_id;
            break;
        case PINB5:
            TRISBbits.RB5 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB5;
            pin->get_id = (pin_get_id_t)&get_PINB5_id;
            break;
        case PINB6:
            TRISBbits.RB6 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB6;
            pin->get_id = (pin_get_id_t)&get_PINB6_id;
            break;
        case PINB7:
            TRISBbits.RB7 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINB7;
            pin->get_id = (pin_get_id_t)&get_PINB7_id;
            break;
        
            
         case PINC0:
            TRISCbits.RC0 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINC0;
            pin->get_id = (pin_get_id_t)&get_PINC0_id;
            break;
        case PINC1:
            TRISCbits.RC1 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINC1;
            pin->get_id = (pin_get_id_t)&get_PINC1_id;
            break;
        case PINC2:
            TRISCbits.RC2 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINC2;
            pin->get_id = (pin_get_id_t)&get_PINC2_id;
            break;  
        case PINC6:
            TRISCbits.RC6 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINC6;
            pin->get_id = (pin_get_id_t)&get_PINC6_id;
            break;
        case PINC7:
            TRISCbits.RC7 = OUTPUT;
            pin->write = (digital_output_write_t)&set_PINC7;
            pin->get_id = (pin_get_id_t)&get_PINC7_id;
            break;
    }
}


