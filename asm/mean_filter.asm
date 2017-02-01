;/*
; * mean_filter.asm
; *
; *  Created on: 31.1.2017
; *      Author: Jari Honkanen
; */


	.sect "ramfuncs"
	.global	_mean_filter

_mean_filter:

	MOVW	DP,	#_AdcResult
	ZAPA
	ADD		AH,@_AdcResult+2
	ADD		AH,@_AdcResult+3
	ADD		AH,@_AdcResult+5
	ADD		AH,@_AdcResult+6
	ADD		AH,@_AdcResult+7
	ADD		AH,@_AdcResult+8
	ADD		AH,@_AdcResult+9
	ADD		AH,@_AdcResult+10
	ADD		AH,@_AdcResult+11
	ADD		AH,@_AdcResult+12
	ADD		AH,@_AdcResult+13
	ADD		AH,@_AdcResult+14
	ADD		AH,@_AdcResult+15

	MOV		PH,#0x13b1 ;load multiplicant with 1/13 in Q32 format
	MOV		PL,#0x3b14

	MOVL	XT,P

	QMPYUL	P,XT,ACC
	MOVL	ACC,P
	MOVW	DP, #_current_filter_2_output
	MOV		@_current_filter_2_output,AH

	LRETR
;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_AdcResult
	.global	_ext_ad
	.global _current_samples
	.global _current_filter_2_output
	.global _mean_filt
