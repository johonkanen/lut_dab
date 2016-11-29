;****************************************************************
;* FNAME: uart_stream.asm                Jhonkanen   3.8.2016  	*
;*                                                             	*
;*	assembly routine for streaming data with uart             	*
;*															   	*
;*	Uses registers AL and AH								    *
;*																*
;*                                                             	*
;****************************************************************
;

; stream data via SCI
; ad channels
; _AdcResult:
;0 - ada4, aux voltage		|OK
;1 - adb4, dc link voltage	|OK
;2 - ada1, pfc current		|OK
;3 - adb1, mains voltage	|OK
;4 - ada6, heater current	|OK
;5 - N/A
;6 - ada2, cuk current		|OK
;7 - adb2, cuk cap			|OK
;8 - ada3, Cuk voltage		|OK
;9 - adb3, heater voltage	|OK


; test if tx buffer is empty
        MOVW    DP,#_SciaRegs+10
        AND     AL,@_SciaRegs+10,#0x1f00
        LSR     AL,8
        BF      COMM_NOT_READY,NEQ

;Stream data

;		SPM		1

		MOVW	DP,#_mailbox
		MOVL	XAR1,@_mailbox

		MOVW	DP, #_filtergains
		MOVL	XAR0,#_filtergains

		MOV		AH,*+XAR1[0]
		MOV		AL,#0

		MOVL	XT,*+XAR0[0] ;b1
		QMPYL	P,XT,ACC ;b1*u

		MOVL	XT,*+XAR0[2] ;a1
		QMPYL	ACC,XT,*+XAR0[4] ; y*a1
		NOP

		NEG		ACC ; -y*-a1
		ADDL	ACC,P ; ACC = b1*u-y*a1
		MOVL	*+XAR0[4],ACC ;mem = ACC

		MOV		T,#3
		LSLL	ACC,T
		MOV		T,#56503
		MPYU	ACC,T,AH

		MOV		AH,*+XAR1[0]

        MOVW    DP,#_SciaRegs+9
        MOV     @_SciaRegs+9,AH
        ASR     AH,8

        MOV     @_SciaRegs+9,AH       ; Set AH to pipeline

COMM_NOT_READY:

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
	.global _sig_prbs
	.global	_SCIdata
	.global	_mailbox
	.global _memory
	.global _package
	.global _filtergains
