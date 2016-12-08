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

extern float measgain;
extern float juttu;
extern int16 mail;



#endif /* INTERRUPTS_H_ */




