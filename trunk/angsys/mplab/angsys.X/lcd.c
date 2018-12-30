#include <angsys.h>
#include <stdarg.h>

typedef struct lcd_handle_tag
{
    coord_t size;
    coord_t cursor;
    digital_output_t RS;
    digital_output_t E;
    digital_output_t D4;
    digital_output_t D5;
    digital_output_t D6;
    digital_output_t D7;
}lcd_handle_t;

static lcd_handle_t intern_lcd_s;


void lcd_entry_mode(bool_t id, bool_t scroll);
void lcd_display_control(bool_t display, bool_t cursor, bool_t blink);
void lcd_clear(void);
void lcd_home(void);
void lcd_put(byte value);
void lcd_gotoxy(byte x, byte y);
void lcd_back(void);

void lcd_print(rom far const char*,...);

void lcd_write_low_nibble(byte value)
{
    ang_byte_bits_t b;
    b.value = value;
    
    intern_lcd_s.D4.write(b.bits.bit0);
    intern_lcd_s.D5.write(b.bits.bit1);
    intern_lcd_s.D6.write(b.bits.bit2);
    intern_lcd_s.D7.write(b.bits.bit3);
}

void lcd_write_high_nibble(byte value)
{
    ang_byte_bits_t b;
    b.value = value;
    
    intern_lcd_s.D4.write(b.bits.bit4);
    intern_lcd_s.D5.write(b.bits.bit5);
    intern_lcd_s.D6.write(b.bits.bit6);
    intern_lcd_s.D7.write(b.bits.bit7);
}

void lcd_initialize(byte column, byte row)
{
    intern_lcd_s.size.x = column;
    intern_lcd_s.size.y = row;
    
    delay_ms(31);
    
    intern_lcd_s.RS.write(LOW);
    intern_lcd_s.E.write(LOW);
    
    //LCD_DATA = 0X02;
    lcd_write_low_nibble(0X02);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_ms(5);
    
    //LCD_DATA = 0X02;
    //lcd_write_low_nibble(0X02);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_ms(5);
    
    //LCD_DATA = 0X08;
    lcd_write_low_nibble(0X08);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_ms(5);
    
    lcd_entry_mode(true, false);
    lcd_display_control(true, true, false);
    lcd_clear();
    lcd_home();
     
}

void lcd_entry_mode(bool_t id, bool_t scroll)
{
    intern_lcd_s.RS.write(LOW);
   
    //LCD_DATA = 0X00;
    lcd_write_low_nibble(0X0);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
   
    //LCD_DATA = 0X04;
    intern_lcd_s.D4.write(scroll?HIGH:LOW);
    intern_lcd_s.D5.write(id?HIGH:LOW);
    intern_lcd_s.D6.write(HIGH);
    intern_lcd_s.D7.write(LOW);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_us(40);
}


void lcd_display_control(bool_t display, bool_t cursor, bool_t blink)
{
    intern_lcd_s.RS.write(LOW);
   
    //LCD_DATA = 0X00;
    lcd_write_low_nibble(0X0);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    //LCD_DATA = 0X08;
    intern_lcd_s.D4.write(blink?HIGH:LOW);
    intern_lcd_s.D5.write(cursor?HIGH:LOW);
    intern_lcd_s.D6.write(display?HIGH:LOW);
    intern_lcd_s.D7.write(HIGH);
   
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_us(40);
}

void lcd_clear(void)
{
    intern_lcd_s.RS.write(LOW);
   
    //LCD_DATA = 0X00;
    lcd_write_low_nibble(0X0);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
   
    //LCD_DATA = 0X01;
    lcd_write_low_nibble(0X1);
      
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_ms(2);
}

void lcd_home(void)
{
    intern_lcd_s.RS.write(LOW);
    intern_lcd_s.cursor.x = 0;
    intern_lcd_s.cursor.y = 0;
    //LCD_DATA = 0X00;
    lcd_write_low_nibble(0X0);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
   
    //LCD_DATA = 0X01;
    lcd_write_low_nibble(0X2);
      
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_ms(2);
}

coord_t lcd_get_cursor(void)
{
    return intern_lcd_s.cursor;
}

void lcd_gotoxy(byte x, byte y)
{
    byte address;
    
    if(x >= intern_lcd_s.size.x)
        x = intern_lcd_s.size.x - 1;
    if(y >= intern_lcd_s.size.y)
        y = intern_lcd_s.size.y - 1;
    
    intern_lcd_s.cursor.x = x;
    intern_lcd_s.cursor.y = y;
    
    address = y * 0X40 + x;
    
    intern_lcd_s.RS.write(LOW);  
    
    lcd_write_high_nibble(address);
    intern_lcd_s.D7.write(HIGH);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    lcd_write_low_nibble(address);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_us(40);
}

void lcd_put(byte value)
{ 
    if(value == 0X08)
    {
        lcd_back();
        return;
    }
    else if(value == 0)
    {
        return;
    }
    
    intern_lcd_s.RS.write(HIGH);   
    
    lcd_write_high_nibble(value);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    lcd_write_low_nibble(value);
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_us(100);
    intern_lcd_s.RS.write(LOW); 
    
    intern_lcd_s.cursor.x ++;
    if(intern_lcd_s.cursor.x >= intern_lcd_s.size.x)
    {
        intern_lcd_s.cursor.x = 0;
        intern_lcd_s.cursor.y++;
        if(intern_lcd_s.cursor.y >= intern_lcd_s.size.y)
            intern_lcd_s.cursor.y = 0;      
    }
    
    lcd_gotoxy(intern_lcd_s.cursor.x, intern_lcd_s.cursor.y);
}

void lcd_back(void)
{
    
    intern_lcd_s.cursor.x--;
    if(intern_lcd_s.cursor.x == 0XFF)
    {
        intern_lcd_s.cursor.x = intern_lcd_s.size.x - 1;
        intern_lcd_s.cursor.y--;
        if(intern_lcd_s.cursor.y == 0XFF)
            intern_lcd_s.cursor.y = intern_lcd_s.size.y - 1;      
    }
    
    lcd_gotoxy(intern_lcd_s.cursor.x, intern_lcd_s.cursor.y);
    
    intern_lcd_s.RS.write(HIGH);   
    
    lcd_write_high_nibble(' ');
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    lcd_write_low_nibble(' ');
    
    intern_lcd_s.E.write(HIGH);
    delay_us(2);
    intern_lcd_s.E.write(LOW);
    
    delay_us(100);
    intern_lcd_s.RS.write(LOW); 
    
    lcd_gotoxy(intern_lcd_s.cursor.x, intern_lcd_s.cursor.y);
}

void lcd_write(rom far const char* str)
{
    int c;
    for(c = 0; str[c] != 0; c++)
        lcd_put(str[c]);
}

void lcd_write_ram(const char* str)
{
    int c;
    for(c = 0; str[c] != 0; c++)
        lcd_put(str[c]);
}

char buffer[64];
void lcd_print(rom far const char* format,...)
{
    va_list args;  
    va_start(args,format);                            // Prep arguments list
    vsprintf(buffer,format,args);                        // "Print" to buffer
    va_end(args);
    lcd_write_ram(buffer);
}

bool_t lcd_create(lcd_t* lcd, pin_id_t RS, pin_id_t E, pin_id_t D4, pin_id_t D5, pin_id_t D6, pin_id_t D7)
{
    ang_create_digital_output(RS, &intern_lcd_s.RS);
    ang_create_digital_output(E, &intern_lcd_s.E);
    ang_create_digital_output(D4, &intern_lcd_s.D4);
    ang_create_digital_output(D5, &intern_lcd_s.D5);
    ang_create_digital_output(D6, &intern_lcd_s.D6);
    ang_create_digital_output(D7, &intern_lcd_s.D7);
    
    lcd->id = 0;
    lcd->init = (lcd_init_t)&lcd_initialize;
    lcd->entry = (lcd_entry_mode_t)&lcd_entry_mode;
    lcd->dysplay = (lcd_display_control_t)&lcd_display_control;
    lcd->clear = (lcd_clear_t)&lcd_clear;
    lcd->home = (lcd_home_t)&lcd_home;
    lcd->put = (lcd_put_t)&lcd_put;
    lcd->write = (lcd_write_t)&lcd_write;
    lcd->print = (lcd_print_t)&lcd_print;
    lcd->back = (lcd_back_t)&lcd_back;
    lcd->gotoxy = (lcd_gotoxy_t)&lcd_gotoxy;
    return true;
}
