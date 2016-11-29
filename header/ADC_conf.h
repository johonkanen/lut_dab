/*
 * ADC_conf.h
 *
 *  Created on: 28 Jun 2015
 *      Author: Jari
 */

#ifndef ADC_CONF_H_
#define ADC_CONF_H_

#include "DSP28x_Project.h"
//__interrupt void adc_interrupt_after_startup(void);
__interrupt void startup_filter(void);

__interrupt void AUX_timing(void);
__interrupt void PFC_timing(void);
__interrupt void HEATER_timing(void);

extern struct measurements meas;

void Adc_config(void);
void config_measurements(struct measurements*);

extern int32 startup_filter_num[3];
extern int32 startup_filter_den[3];
extern int32 startup_filter_mem[3];

#endif /* ADC_CONF_H_ */
