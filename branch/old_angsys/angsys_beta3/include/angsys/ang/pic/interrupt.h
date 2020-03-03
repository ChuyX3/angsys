/* 
 * File:   interrupt.h
 * Author: Jesus Rocha
 *
 * Created on December 12, 2018, 4:19 PM
 */

#ifndef __ANG_C18_INTERRUPT_H__
#define	__ANG_C18_INTERRUPT_H__

ANG_EXTERN void interrupt_set_low_level_callback(lpfunc_t callback);
ANG_EXTERN void interrupt_set_high_level_callback(lpfunc_t callback);

#endif//__ANG_C18_INTERRUPT_H__

