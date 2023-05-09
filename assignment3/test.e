	push	1000
	pop	sp
	push	"Please enter a number: "
	pop	sb[7]
	push	sb[7]
	puts
	geti
	pop	sb[0]
	push	0
	pop	sb[8]
L000:
	push	sb[8]
	push	6
	complt
	j0	L001
	push	0
	push	0
	add
	pop	in
	push	sb[in]
	push	sb[8]
	mul
	push	sb[8]
	push	1
	add
	pop	in
	pop	sb[in]
	push	sb[8]
	push	1
	add
	pop	sb[8]
	jmp	L000
L001:
	push	"The result is:"
	puts
	push	0
	pop	sb[8]
L002:
	push	sb[8]
	push	6
	complt
	j0	L003
	push	sb[8]
	push	1
	add
	pop	in
	push	sb[in]
	puti_
	push	' '
	putc_
	push	sb[8]
	push	1
	add
	pop	sb[8]
	jmp	L002
L003:
	push	""
	puts
