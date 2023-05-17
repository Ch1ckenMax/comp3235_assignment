	push	1000
	pop	sp
	jmp	L001
L000:
	var	2,0
	push	fp[0]
	push	fp[1]
	push	fp[0]
	push	fp[1]
	add
	RET
L001:
	jmp	L003
L002:
	var	2,0
	push	fp[0]
	push	fp[1]
	push	fp[0]
	push	fp[1]
	sub
	RET
L003:
	push	2
	push	3
	CALL	L000
	push	2
	push	1
	CALL	L002
	sub
	puti
