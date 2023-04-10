lex.yy.c: q2.l
	flex q2.l

q2.tab.c: q2.y
	bison q2.y

q2: q2.tab.c lex.yy.c
	gcc q2.tab.c -o q2

.PHONY: clean
clean: 
	rm lex.yy.c q2 q2.tab.c