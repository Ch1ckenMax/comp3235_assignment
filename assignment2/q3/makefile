c4c: lex.yy.c y.tab.c c4c.c
	gcc -o c4c lex.yy.c y.tab.c c4c.c

lex.yy.c: c4.l
	flex c4.l

y.tab.c: c4.y
	bison -y -d c4.y

nas: nas.c nas.tab.c
	gcc -o nas nas.c nas.tab.c

nas.c: nas.l
	flex -o nas.c nas.l

nas.tab.c: nas.y
	bison -d nas.y

cleanc4:
	$(RM) lex.yy.c y.tab.c y.tab.h c4c

cleannas:
	$(RM) nas.c nas.tab.c nas.tab.h nas
