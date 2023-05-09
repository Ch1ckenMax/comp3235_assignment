	push	1000
	pop	sp
	push	3
	pop	sb[10]
	push	5
	pop	sb[11]
	push	"This is a fucking array!"
	pop	sb[12]
	push	0
	pop	sb[13]
L000:
	push	sb[13]
	push	5
	complt
	j0	L001
	push	sb[13]
	push	0
	add
	pop	in
	push	sb[13]
	push	1
	add
	pop	sb[in]
	push	sb[13]
	push	5
	add
	pop	in
	push	sb[13]
	push	1
	sub
	pop	sb[in]
	push	sb[13]
	push	1
	add
	pop	sb[13]
	jmp	L000
L001:
	push	0
	pop	sb[13]
L002:
	push	sb[13]
	push	5
	complt
	j0	L003
	push	sb[13]
	push	0
	add
	pop	in
	push	sb[in]
	push	sb[10]
	mul
	puti
	push	sb[13]
	push	5
	add
	pop	in
	push	sb[in]
	push	sb[11]
	mul
	puti
	push	sb[13]
	push	1
	add
	pop	sb[13]
	jmp	L002
L003:
