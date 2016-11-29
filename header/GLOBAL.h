/*
 * GLOBAL.h
 *
 *  Created on: 30.7.2015
 *      Author: jhonkanen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_


extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
/*

extern Uint16 Cla1funcsLoadStart;
extern Uint16 Cla1funcsRunStart;
extern Uint16 Cla1funcsLoadSize;

*/
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
	//aux_measurement
	Uint16* aux_voltage;
	//pfc measurements
	Uint16* dc_link_voltage;
	Uint16* pfc_current1;
	Uint16* pfc_current2;
	Uint16* mains_voltage;
	//dhb_measurements
	Uint16* dhb_current;
	Uint16* dhb_output_voltage;
	Uint16* dhb_cap_voltage;
	//heater_measurements
	Uint16* heater_voltage;
	Uint16* heater_current;
};

struct storage
{
	Uint16 rdptr;
	Uint16 wrptr;
	Uint32* mem;
};
/*
Uint32 prbs_[512] =
{
#include "prbs14_parsed_32bit.txt"
};
*/

Uint16	SCIdata;

#endif /* GLOBAL_H_ */




