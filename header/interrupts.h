/*
 * interrupts.h
 *
 *  Created on: 8.12.2016
 *      Author: jhonkanen
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "DSP28x_Project.h"
__interrupt void PWM1_int(void);

void read_ext_ad(void);
extern Uint16 cnt_jee;
extern struct measurements meas;

extern float juttu;
extern int16 mail;


#define duty_ch(X) ((*ePWM[X]).CMPA.half.CMPA)



//Uint16 ph_shift;

#endif /* INTERRUPTS_H_ */




