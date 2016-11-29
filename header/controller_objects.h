/*
 * controller_objects.h
 *
 *  Created on: 30.7.2015
 *      Author: c0341878
 */

#ifndef CONTROLLER_OBJECTS_H_
#define CONTROLLER_OBJECTS_H_

PI_controller AUX_voltage_ctrl;		//PI control structure for DCM flyback converter
PI_controller DC_link_voltage_ctrl;

__interrupt void AUX_ctrl(void);
__interrupt void AUX_ctrl_cuk_rampup(void);

#endif /* CONTROLLER_OBJECTS_H_ */
