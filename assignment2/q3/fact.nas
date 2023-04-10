	geti
	pop	sb[23]
	push	sb[23]
	push	0
	compeq
	j0	L000
	push	'c'
	puti
	jmp	L001
L000:
	push	sb[23]
	push	1
	compge
	push	sb[23]
	push	12
	comple
	and
	j0	L002
	push	1
	pop	sb[5]
L003:
	push	sb[23]
	push	1
	compgt
	j0	L004
	push	sb[5]
	push	sb[23]
	mul
	pop	sb[5]
	push	sb[23]
	push	1
	sub
	pop	sb[23]
	jmp	L003
L004:
	push	sb[5]
	puti
L002:
L001:
