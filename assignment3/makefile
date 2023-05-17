c6c: lex.yy.c y.tab.c c6c.c
	gcc -g -o c6c lex.yy.c y.tab.c c6c.c

lex.yy.c: c6.l
	flex c6.l

y.tab.c: c6.y
	bison -y -d c6.y

nas:
	flex nas.l
	bison -d nas.y
	gcc -o nas lex.yy.c nas.tab.c

nas2:
	flex nas2.l
	bison -d nas2.y
	gcc -o nas2 lex.yy.c nas2.tab.c

cleanc6:
	$(RM) lex.yy.c y.tab.c y.tab.h c6c

cleannas:
	$(RM) nas.c nas.tab.c nas.tab.h nas

cleannas2:
	$(RM) nas2.c nas2.tab.c nas2.tab.h nas2
