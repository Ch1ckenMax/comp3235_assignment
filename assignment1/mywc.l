/*
 * wc - word (and line and char) count
 * flex mywc.l ; gcc -o mywc lex.yy.c ; ./mywc
 */

%option noyywrap

%{
  int chars = 0;
  int words = 0;
  int lines = 0;
  int paragraphs = 0;
%}

%%

[^\n\t\v\b ]+	{ words++; chars += yyleng; } /* According to the linux manual, wc counts words that are "non-zero-length sequence of characters delimited by white space". Hence, I am just matching all strings that is not a space character, not a new-line chracater, and not any of the space-like characters (e.g. tab) */
[\t\v\b ]+ { chars += yyleng; }

\n		{ lines++; chars++; } /* newline without adding new paragraphs */

\n\n+ { paragraphs++; lines += yyleng; chars += yyleng; } /* I assume that if there is more than two newline, then there is a new paragraph */
<<EOF>> { paragraphs++; yyterminate(); } /* There is usually no newline character for the last paragraph */
 

%%

int main(int argc, char *argv[]) {
  if ( argc > 1 ) {
    if ( (yyin = fopen(argv[1], "r")) == 0 ) {
      perror(argv[1]);
      exit(1);
    }
  }
  yylex();
  printf("L=%d W=%d C=%d P=%d\n", lines, words, chars, paragraphs);
  exit(0);
}
