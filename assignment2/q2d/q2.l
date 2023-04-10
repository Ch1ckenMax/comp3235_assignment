%{
void yyerror(char *);
%}

%option noyywrap

%%

[ab\[\]] return *yytext;
\n       {} /* prevents newline from printing to stdout */
.        yyerror(NULL);

%%