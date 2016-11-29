;****************************************************************
;* FNAME: AUX_PI_ctrl.asm        Jhonkanen   2.7.2016		  	*
;*																*
;*                                                             	*
;****************************************************************


aux_max_duty .set 222
sat_aux_high .set 221

aux_min_duty .set 5
sat_aux_min  .set 4

;remove spurious conversions

;		MOVW	DP,#_memory
;		MOV 	AH,@_memory

;        MOVW    DP,#_AdcResult          ; ADCRESULT 0 = AUX voltage
;		MOV		AL,@_AdcResult+0

;		CMP		AH,AL
;		MOV		AR0,AH,HI
;		MOV		AR0,AL,LOS

;		MOVW	DP,#_memory
;		MOV 	@_memory,AL


;Load controller register
        MOVW    DP,#_AUX_voltage_ctrl			;
        MOVL	XAR1,#_AUX_voltage_ctrl			;XAR1 = PI control pointer
 		MOV		AH,@_AUX_voltage_ctrl+6			;AH = 16 bit reference, even since 16bit

;calculate error = ref-adc0

        MOVW    DP,#_aux_meas          ; ADCRESULT 8 = AUX voltage
        MOVL	XAR2, @_aux_meas
 		SUB		AH,*+XAR2[0]		;ACC = error<<16
;		SUB		AH,AR0
 		LSL		ACC,#2
 		MOVL	XT,ACC					;XT = err

;*****************************************
;send error to sci
;*****************************************


;PI controller static equation
		QMPYL	P,XT,*+XAR1[1]			;P = e*P_gain
		MOVL	ACC,*+XAR1[4]			;ACC = mem[32bit] [4] and [5] is the same if MOVL is used
		ADDL	P,ACC					;P = output

;check over/underflow
		CMP		PH, #aux_max_duty
		BF		SAT_HIGH,GEQ

		CMP		PH, #aux_min_duty
		BF		SAT_LOW,LEQ

;NORMAL OPERATION
		QMPYL	ACC,XT,*+XAR1[2]
		NOP
		ADDL	*+XAR1[4],ACC
		BF		UPDATE_PWM,UNC
SAT_HIGH:
		MOVL	ACC,XT
		ABS		ACC
		MOV		T,#15
		ASRL	ACC,T
		NEG		ACC

		ADDL	*+XAR1[4],ACC
		MOV		PH,#sat_aux_high
		BF		UPDATE_PWM,UNC

SAT_LOW:
		MOVL	ACC,XT
		ABS		ACC
;//		NEG		ACC
		NOP		;to normalize sat low,high cycles

		MOV		T,#15
		ASRL	ACC,T
		ADDL	*+XAR1[4],ACC
		MOV		PH,#sat_aux_min
		BF		UPDATE_PWM,UNC

;update AUX PWM with PI controller output
UPDATE_PWM:
        MOVW    DP,#_ePWM+0
        MOVL    XAR4,@_ePWM+0
        MOVB	XAR0,#0x9		; CMPA offset  == 9
;HRPWM UPDATE
		MOV		T,#0x6f00
		MPYU	ACC,T,PL
		MOV 	PL,AH
;duty = 1-duty
		MOVL	ACC,P
		SUB		AH,#443
		NEG		AH

		SUB		AL,#0x6f00
		NEG		AL

        MOVL	*+XAR4[AR0],ACC

;channel B update

        MOVB	XAR0,#0xA		; CMPB offset  == 10
        MOV		AH,#300
        MOV		*+XAR4[AR0],AH

;end hrpwm update


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
	.global _startup_voltage
	.global _ePWM
	.global _AUX_voltage_ctrl
	.global _EPwm1Regs
	.global	_GPIO
	.global _memory
	.global _aux_meas
