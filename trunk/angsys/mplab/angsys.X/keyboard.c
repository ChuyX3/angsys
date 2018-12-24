#include <angsys.h>

typedef struct keyboard_handle_tag
{
    digital_output_t DO0;
    digital_output_t DO1;
    digital_output_t DO2;
    digital_output_t DO3;
    
    digital_input_t DI0;
    digital_input_t DI1;
    digital_input_t DI2;
    digital_input_t DI3;
}keyboard_handle_t;

static keyboard_handle_t intern_keyboard_s;

char keyboard_get(void);
char keyboard_get_raw(void);

static rom const char keys[4][4] = {
    {'1', '2', '3', '/'},
    {'4', '5', '6', '*'},
    {'7', '8', '9', '-'},
    {'.', '0', '=', '+'}
};

void keyboard_create(keyboard_t* keyboard,
        pin_id_t DO0, pin_id_t DO1, pin_id_t DO2, pin_id_t DO3,
        pin_id_t DI0, pin_id_t DI1, pin_id_t DI2, pin_id_t DI3
    )
{
    ang_create_digital_output(DO0, &intern_keyboard_s.DO0);
    ang_create_digital_output(DO1, &intern_keyboard_s.DO1);
    ang_create_digital_output(DO2, &intern_keyboard_s.DO2);
    ang_create_digital_output(DO3, &intern_keyboard_s.DO3);
    
    ang_create_digital_input(DI0, &intern_keyboard_s.DI0);
    ang_create_digital_input(DI1, &intern_keyboard_s.DI1);
    ang_create_digital_input(DI2, &intern_keyboard_s.DI2);
    ang_create_digital_input(DI3, &intern_keyboard_s.DI3);
    
    keyboard->get = (keyboard_get_t)&keyboard_get;
}


void keyboard_write_nibble(byte value)
{
    ang_byte_bits_t bits;
    bits.value = value;
    intern_keyboard_s.DO0.write(bits.bits.bit0);
    intern_keyboard_s.DO1.write(bits.bits.bit1);
    intern_keyboard_s.DO2.write(bits.bits.bit2);
    intern_keyboard_s.DO3.write(bits.bits.bit3);
}

char keyboard_get_ascii(void)
{
    char c, a;
    c = keyboard_get_raw();
    if(c >= '0' && c <= '9')
        c = c - '0';
    else if(c >= 'A' && c <= 'D')
        c = c - 'A' + 10;
    else
        return 0;
    a = 0X10 * c;
    c = keyboard_get_raw();
    if(c >= '0' && c <= '9')
        c = c - '0';
    else if(c >= 'A' && c <= 'D')
        c = c - 'A' + 10;
    else
        return 0;
    
    a += c;
    return a;
}

char keyboard_get(void)
{
    char c;
    c = keyboard_get_raw();
    
    if(c == '#')
    {
        c = keyboard_get_ascii();
    }
  //  else if(c == '*')
  //  {
  //      c = 0X08;
  //  }
    return c;
}

char keyboard_get_raw(void)
{
    byte i, j = 0;
    while(true)
    {
        for(i = 0; i < 4; i++)
        {
            keyboard_write_nibble(~(1 << i));
            delay_ms(1);
            if(intern_keyboard_s.DI0.read() == LOW) {
                //while(intern_keyboard_s.DI0.read() == LOW); //wait for key up
                delay_ms(250); //
                return keys[0][i];
            }
            else if(intern_keyboard_s.DI1.read() == LOW) {
                //while(intern_keyboard_s.DI1.read() == LOW); //wait for key up
                delay_ms(250); //
                return keys[1][i];
            }
            else if(intern_keyboard_s.DI2.read() == LOW) {
                //while(intern_keyboard_s.DI2.read() == LOW); //wait for key up
                delay_ms(250); //
                return keys[2][i];
            }
            else if(intern_keyboard_s.DI3.read() == LOW) {
                //while(intern_keyboard_s.DI3.read() == LOW); //wait for key up
                delay_ms(250); //
                return keys[3][i];
            }

        }//for
    }//while
}