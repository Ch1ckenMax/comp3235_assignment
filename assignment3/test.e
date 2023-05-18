	push	1000
	pop	sp
	push	5
	pop	sb[0]
	push	7
	pop	sb[1]
	push	10
	push	0
	push	2
	add
	pop	in
	pop	sb[in]
	push	20
	push	1
	push	2
	add
	pop	in
	pop	sb[in]
	push	3
	push	2
	push	2
	add
	pop	in
	pop	sb[in]
	push	"yes"
	pop	sb[5]
	jmp	L003
L000:
	var	0,1
	push	fp[0]
	push	sb[1]
	compgt
	j0	L001
	push	0
	push	2
	add
	pop	in
	push	sb[in]
	RET
	jmp	L002
L001:
	push	1
	push	2
	add
	pop	in
	push	sb[in]
	RET
L002:
L003:
	push	8
	CALL	L000
	puti
	jmp	L005
L004:
	var	0,1
	push	5
	pop	fp[0]
	CALL	L004
	RET
L005:
	push	2
	push	2
	add
	pop	in
	push	sb[in]
	CALL	L004
	add
	puti
	push	"Yay such my ball mipah"
	puts
