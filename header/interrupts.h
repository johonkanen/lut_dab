/*
 * interrupts.h
 *
 *  Created on: 8.12.2016
 *      Author: jhonkanen
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#ifndef INTERRUPTS_C
#define EXT extern
#else
#define EXT
#endif

#include "DSP28x_Project.h"
__interrupt void PWM1_int(void);

void read_ext_ad(void);
extern Uint16 cnt_jee;
extern struct measurements meas;

extern float juttu;
extern int16 mail;

extern float measgain;

#define duty_ch(X) ((*ePWM[X]).CMPA.half.CMPA)

#define pri_current (*meas.pri_current_1)

EXT Uint16 ph_shift_1;
EXT Uint16 ph_shift_2;

EXT Uint16 ph_shift_3;
EXT Uint16 ph_shift_4;

EXT Uint16 ph_shift_pri_sec_1;
EXT Uint16 ph_shift_pri_sec_2;

#endif /* INTERRUPTS_H_ */




