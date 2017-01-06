/*
 * GLOBAL.h
 *
 *  Created on: 1.12.2016
 *      Author: jhonkanen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

void rampup(void);
void rampup_init(void);

extern void init_cla(void);
/**********************************/
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

extern volatile struct EPWM_REGS *ePWM[9];
float measgain = (float)0.109890109890110;

struct f_pid_ctrl voltage_ctrl;

struct f_pid_ctrl d1_ctrl;
struct f_pid_ctrl d2_ctrl;

struct storage sig_prbs;
struct measurements meas;
struct ext_ad_result ext_ad;
struct pwm_interface phase_reg;


Uint16	SCIdata;

int16 cnt_jee=0;

Uint16* mailbox;

#endif /* GLOBAL_H_ */




