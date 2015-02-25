
	case on

	export nan, __inf, inf

nan	proc
	; The MPW 3.2 version of nan is broken.  This what it's supposed to do.
	; extended (80-bit) returns via d0, d1, a0 

	MOVEM.L    data,D0/D1/A0
	MOVE.W     #$4000,D1
	MOVE.B     $0004(A7),D1
	BNE.S swap
	MOVE.B #$15,D1
swap
	SWAP D1
	RTS

	entry inf
	entry __inf

inf
__inf

	MOVEM.L    data,D0/D1/A0
	RTS

data
	dc.l $00007fff
	dc.l $00000000
	dc.l $00000000     

	end
	endp

