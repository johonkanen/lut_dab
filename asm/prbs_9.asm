;****************************************************************
;* FNAME: prbs_9.asm                Jhonkanen   31.1.2017 	*
;*                                                             	*
;*	assembly routine for generating 9bit prbs	             	*
;*															   	*
;*	Uses registers AL and AH								    *
;*																*
;*                                                             	*
;****************************************************************
;PRBS generation
    .sect "ramfuncs"
	.global	_prbs_9

_prbs_9:
		MOVW	DP,#_sig_prbs
		MOV		AL,@_sig_prbs+0	;load wrptr
		MOV		AH,@_sig_prbs+0


		LSL		AL,#4

		XOR		AH,AL
		AND		AH,#0x200
		LSR		AL,#5

		OR		AL,AH


;prbs is ready
		MOV		@_sig_prbs+0,AL

		LRETR
;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_sig_prbs
