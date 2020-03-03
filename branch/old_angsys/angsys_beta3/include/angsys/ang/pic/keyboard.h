/* 
 * File:   keyboard.h
 * Author: Jesus Rocha
 *
 * Created on December 13, 2018, 6:39 PM
 */

#ifndef __ANG_C18_KEYBOARD_H__
#define	__ANG_C18_KEYBOARD_H__

typedef struct keyboard_tag keyboard_t;
typedef rom far char (*keyboard_get_t)(void);

struct keyboard_tag
{
    keyboard_get_t get;
};

ANG_EXTERN void keyboard_create(keyboard_t*,
        pin_id_t DO0, pin_id_t DO1, pin_id_t DO2, pin_id_t DO3,
        pin_id_t DI0, pin_id_t DI1, pin_id_t DI2, pin_id_t DI3
);


#endif//__ANG_C18_KEYBOARD_H__

