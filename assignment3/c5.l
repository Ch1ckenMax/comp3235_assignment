%{
#include <stdlib.h>
#include <string.h>
#include "calc3.h"
#include "y.tab.h"
void yyerror(char *);
%}

%%

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-\[\],()<>=+*/;{}.%] {
                return *yytext;
             }

\'.\'       {
                yylval.iValue = yytext[1];
                return CHARACTER;
            }

\".*\"      {
                yylval.sPtr = strdup(yytext);
                return STRING;
            }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"for"		return FOR;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"&&"		return AND;
"||"		return OR;
"geti"      return GETI;
"getc"      return GETC;
"gets"      return GETS;
"puti_"     return PUTI_;
"putc_"     return PUTC_;
"puts_"    return PUTS_;
"puti"      return PUTI;
"putc"      return PUTC;
"puts"      return PUTS;
"array"     return ARRAY;


[a-zA-Z][a-zA-Z0-9]*       { 
                yylval.sPtr = strdup(yytext);
                return VARIABLE;
            }

\/\/.*\n	;	/* skip comments */

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
