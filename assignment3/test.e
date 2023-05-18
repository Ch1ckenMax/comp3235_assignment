	push	1000
	pop	sp
	jmp	L001
L000:
	var	1,1
	push	4
	pop	fp[1]
	push	fp[0]
	push	fp[1]
	add
	RET
L001:
	push	4
	CALL	L000
	puti
