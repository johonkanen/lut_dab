;this 'ere is a assembly file

		.def _rampup_init
		.text ;change this to "ramfuncs" if system is booted from flash
		
		.def _rampup
		.text ;change this to "ramfuncs" if system is booted from flash

_rampup_init:

    SPM     0

    MOVW    DP,#_DC_link_voltage_ctrl			;
    MOVL	XAR1,#_DC_link_voltage_ctrl+6		;XAR1 = pointer to PI control ref

;calculate error = ref-adc1
    MOVW    DP,#_AdcResult          ; ADCRESULT 1 = DC link voltage
 	MOV		AH,@_AdcResult+1
 	MOV		*+XAR1[1],AH

	MOV		AL,#0
	MOVL	P,*+XAR1[4]
	MOVL	*+XAR1[4],ACC

	MOVW	DP, #_scale
	MOVL	XT,@_scale

	SUB		AH,*+XAR1[0]
	NEG		AH
	MOVL	P,ACC

	QMPYUL	P,XT,P
	MOVL	ACC,P
	MOV		T,#1

	LSLL	ACC,T

;	MOVW	DP,#_step
	MOV		*+XAR1[2],AL	

	LRETR
	
	

_rampup:

	MOVW	DP,#_rampmem
	MOVL	XAR1,#_rampmem
	MOV		AH,#0

	MOVW	DP,#_step
	MOV		AL,@_step
	ADDL	*+XAR1[0],ACC
	MOVL	ACC,*+XAR1[0]

	MOVW	DP,#_ramptest

	MOV		@_ramptest,AH

	LRETR
; external references:

	.global	_AdcResult
	.global	_ePWM
	.global	_testi
	.global _msg
	.global _sinegain
	.global _ref1
	.global _ref2
	.global _scale
	.global _step
	.global	_rampmem
	.global _ramptest
	.global _DC_link_voltage_ctrl
