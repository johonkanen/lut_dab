;****************************************************************
;* FNAME: heater_modulator_test_adb3.asm  Jhonkanen   26.6.2016	*
;*																*
;*                                                             	*
;****************************************************************

heater_max_duty .set 111
sat_heater_high .set 110

pfc_min_duty .set 10
sat_pfc_min  .set 9

; Heater modulator test

		MOV		DP, #_ePWM
		MOVL	XAR5, @_ePWM+10			;XAR5 = epwm 5
		MOVL	XAR6, @_ePWM+12			;XAR6 = epwm 6

        MOVW    DP,#_AdcResult          ; ADCRESULT 5 = ada5/heater current
 		MOV		AH,@_AdcResult+4		;

 		MOV		T,AH					;XT = err

;HRPWM update
		MOV		AH,#3552
		MPYU	P,T,AH					;P = e*P_gain
		MOV		T,PL
		MOV		AH,#0x7400
		MPYU	ACC,T,AH
		MOV		PL,AH
		MOVL	ACC,P

		MOVB	XAR0,#0x9				; CMPA offset  == 9
		MOVL	*+XAR6[AR0],ACC
		SUB		AH,#14
		MOVL	*+XAR5[AR0],ACC
		MOVB	XAR0,#0xA
		LSR		AH,#1
		MOV		*+XAR4[AR0],AH





;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_SciaRegs
	.global	_PieCtrlRegs

	.global	_AdcResult
	.global	_Voltage1
	.global	_Voltage2
	.global	_AdcRegs
	.global	_ConversionCount
	.global	_startup_filter_mem
	.global _startup_filter_den
	.global _startup_filter_num
	.global _voltage_has_risen
	.global _startup_voltage
	.global _ePWM
	.global _AUX_voltage_ctrl
	.global _EPwm1Regs
	.global	_GPIO
	.global _DC_link_voltage_ctrl
