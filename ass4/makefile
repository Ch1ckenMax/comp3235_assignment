bcc: lex.yy.c y.tab.c bcc.c
	gcc -o bcc lex.yy.c y.tab.c bcc.c

lex.yy.c: bc.l
	flex bc.l

y.tab.c: bc.y
	bison -y -d bc.y

cleanbc:
	$(RM) lex.yy.c y.tab.c y.tab.h bc

