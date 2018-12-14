/* 
 * File:   lcd.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 10:31 PM
 */

#ifndef __ANG_C18_LCD_H__
#define	__ANG_C18_LCD_H__

typedef struct lcd_tag lcd_t;

typedef rom far void (*lcd_init_t)(byte column, byte row);
typedef rom far void (*lcd_entry_mode_t)(volatile near bool_t id, volatile near bool_t scroll);
typedef rom far void (*lcd_display_control_t)(volatile near bool_t display, volatile near bool_t cursor, volatile near bool_t blink);
typedef rom far void (*lcd_clear_t)(void);
typedef rom far void (*lcd_home_t)(void);
typedef rom far void (*lcd_put_t)(volatile near byte value);
typedef rom far void (*lcd_write_t)(rom far const char*);
typedef rom far void (*lcd_gotoxy_t)(byte, byte);

struct lcd_tag
{
    uint id;
    lcd_init_t init;
    lcd_entry_mode_t entry;
    lcd_display_control_t dysplay;
    lcd_clear_t clear;
    lcd_home_t home;
    lcd_put_t put;
    lcd_write_t write;
    lcd_gotoxy_t gotoxy;
};


ANG_EXTERN bool lcd_create(lcd_t* lcd, pin_id_t RS, pin_id_t E, pin_id_t D4, pin_id_t D5, pin_id_t D6, pin_id_t D7);


#endif//__ANG_C18_LCD_H__

