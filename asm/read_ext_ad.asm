;****************************************************************
;* FNAME: _read_ext_ad               Jhonkanen   7.12.2016     	*
;*                                                             	*
;* external ad read sourcefile									*
;*                                                             	*
;****************************************************************

    .sect "ramfuncs"
	.global	_read_ext_ad

_read_ext_ad:

	ZAPA
	MOVW	DP,#_ext_ad
	MOVL	XAR0,#_ext_ad

	MOVW	DP,#_GpioDataRegs

	OR		@_GpioDataRegs+0xa, #0x80
	RPT		#4
	||NOP
	OR		@_GpioDataRegs+0xc, #0x80

	RPT		#6
	||NOP


	MOVL	ACC,@_GpioDataRegs+0 ;ACC = GPADAT

	LSR		AH,#6		; move bits 0-6 to AH0
	MOV		PL,AH		;PL has bits 0-6
	AND		PL,#0x37F	;zero unused bits

	LSR		AL,#6
	AND		AL,#0x80
	OR		PL,AL

	MOVL	ACC,@_GpioDataRegs+8 ;read GPB data

	AND		AL,#3
	LSL		AL,#0xa

	OR		PL,AL
	XOR		PL,#0x800

;latch second conversion

	OR		@_GpioDataRegs+0xa, #0x80
	RPT		#4
	||NOP
	OR		@_GpioDataRegs+0xc, #0x80

	RPT		#6
	||NOP
;read second conversion


	MOVL	ACC,@_GpioDataRegs+0 ;ACC = GPADAT

	LSR		AH,#6		; move bits 0-6 to AH0
	MOV		PH,AH		;PL has bits 0-6
	AND		PH,#0x37F	;zero unused bits

	LSR		AL,#6		;shift bit 7 to proper p
	AND		AL,#0x80
	OR		PH,AL

	MOVL	ACC,@_GpioDataRegs+8 ;read GPB data

	AND		AL,#3
	LSL		AL,#0xa

	OR		PH,AL
	XOR		PH,#0x800

; move data to ext_ad

	MOVL	*+XAR0[0],P

	LRETR
;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_GpioDataRegs
	.global	_ext_ad
