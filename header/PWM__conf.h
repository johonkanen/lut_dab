/*
 * PWM__conf.h
 *
 *  Created on: 28 Jun 2015
 *      Author: Jari
 */

#ifndef PWM__CONF_H_
#define PWM__CONF_H_

#include "DSP28x_Project.h"

void PWM_config(void);

void AUX_PWM1_config(void);

void PRI_DAB_PWM34_config(void);
void SEC_DAB_PWM56_config(void);

void AUX_PWM8_config(void);
void AUX_PWM7_config(void);

void pwm_ad_synch(void);

#endif /* PWM__CONF_H_ */
