/*
 * Gpio_init.h
 *
 *  Created on: 29 Nov 2016
 *      Author: Jari
 */

#ifndef GPIO_INIT_H_
#define GPIO_INIT_H_
#include "DSP28x_Project.h"

void init_lut_dab_GPIO(void);

void init_PFC_PWM4_GPIO(void);
void init_AUX_PWM1_GPIO(void);
void init_DHB_PWM23_GPIO(void);
void init_HEATER_PWM56_GPIO(void);
void enable_DHB_gate_drivers(void);

void init_pri_HB_GPIO(void);
void init_sec_HB_GPIO(void);



#endif /* GPIO_INIT_H_ */
