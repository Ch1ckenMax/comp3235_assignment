bcc: lex.yy.c y.tab.c bcc.c
	gcc -o bcc lex.yy.c y.tab.c bcc.c

lex.yy.c: bc.l
	flex bc.l

y.tab.c: bc.y
	bison -y -d bc.y

blocks:
	gcc -o blocks blocks.c

task2: task2.b bcc
	./bcc <task2.b >task2

task3: task3.b bcc
	./bcc <task3.b >task3

clean:
	$(RM) lex.yy.c y.tab.c y.tab.h bcc task2 task3 blocks
