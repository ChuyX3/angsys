/* 
 * File:   interrupt.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 4:19 PM
 */

#ifndef __ANG_C18_INTERRUPT_H__
#define	__ANG_C18_INTERRUPT_H__

ANG_EXTERN void interrupt_initialize(bool priori, lpfunc_t high_int_vect, lpfunc_t low_int_vect);

#endif//__ANG_C18_INTERRUPT_H__

