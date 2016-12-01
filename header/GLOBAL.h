/*
 * GLOBAL.h
 *
 *  Created on: 1.12.2016
 *      Author: jhonkanen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

Uint16 startup_voltage;

// ramp up signals

Uint16 cuk_rampup;
Uint16 heater_rampup;
Uint16 Aux_rampup;

// converter ready signal

Uint16 timing_complete;
Uint16 voltage_has_risen;
Uint16 dclink_ready;
Uint16 cuk_ready;

int32 startup_filter_num[] = {0x00000000,0x40000000, 0x33E70ABC};
int32 startup_filter_den[] = {0x40000000,0xA2822A24, 0x2224AB3C};
int32 startup_filter_mem[] = {0,0,0};

struct measurements
{
	//pri measurements
	Uint16* pri_current_lp;
	Uint16* pri_current_1;
	Uint16* pri_current_2;
	Uint16* pri_voltage;
	//sec measurements
	Uint16* sec_current;
	Uint16* sec_voltage;
};

struct storage
{
	Uint16 rdptr;
	Uint16 wrptr;
	Uint32* mem;
};
Uint16	SCIdata;

#endif /* GLOBAL_H_ */




