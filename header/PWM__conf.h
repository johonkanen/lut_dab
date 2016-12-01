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

void PRI_DAB_PWM34_config(void);
void SEC_DAB_PWM56_config(void);

void AUX_PWM8_config(void);
void AUX_PWM7_config(void);


void AUX_PWM1_config(void);
void DHB_PWM23_config(void);
void PFC_PWM4_config(void);
void HEATER_PWM56_config(void);


void init_PFC_PWM4_GPIO(void);
void init_AUX_PWM1_GPIO(void);
void init_DHB_PWM23_GPIO(void);
void init_HEATER_PWM56_GPIO(void);
void enable_DHB_gate_drivers(void);





void init_pri_HB_GPIO(void);
void init_sec_HB_GPIO(void);

void pwm_ad_synch(void);


//#define AUX_PWM1B_ENABLE
//#define CUK_PWM2B_ENABLE
//#define CUK_PWM3B_ENABLE
//#define PFC_PWM4B_ENABLE
//#define HEATER_PWM5B_ENABLE
//#define HEATER_PWM6B_ENABLE


#endif /* PWM__CONF_H_ */
