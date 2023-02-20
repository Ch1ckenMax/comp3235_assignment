/*
 * wc - word (and line and char) count
 * flex mywc.l ; gcc -o mywc lex.yy.c ; ./mywc
 */

%option noyywrap

%{
  int chars = 0;
  int words = 0;
  int lines = 0;
%}

%%

[[:alnum:]'\-]+	{ words++; chars += yyleng; }
\n		{ lines++; chars++; } 
.		{ chars++; } 

%%

int main(int argc, char *argv[]) {
  if ( argc > 1 ) {
    if ( (yyin = fopen(argv[1], "r")) == 0 ) {
      perror(argv[1]);
      exit(1);
    }
  }
  yylex();
  printf("L=%d W=%d C=%d\n", lines, words, chars);
  exit(0);
}
