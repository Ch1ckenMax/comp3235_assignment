%{

#include <stdio.h>
#include "lex.yy.c"
#include <stdbool.h>

#define YYSTYPE char*

/*defines the max size of the output string*/
const unsigned int MAX_STR_SIZE = 4096;

/* indicates whether there is error caught in the scanner or parser */
bool hasError = false;

/* stores the pointer to the output string */
char* outputString = NULL;

/* Function prototypes to get rid of the compilation error */
int yylex();
void yyerror(char* s);

%}

%token 'a' 'b' '[' ']'
%%

START:
    S                   { outputString = $1; }
S: 
    S 'a'               { 
                          strcat($1, "a"); $$ = $1;
                        }
    | S 'b'             { 
                          strcat($1, "b"); $$ = $1;
                        }
    | S '[' S ']'       { 
                          strcat($1, $3); 
                          strcat($1, $3); 
                          free($3); /* we do not need (and will not access) the string of the second S anymore. Free it to prevent memory leak.*/
                          $$ = $1;
                        }
    |                   { 
                          /* Create an empty string that can store MAX_STR_SIZE characters. */
                          char* str = (char*) malloc(MAX_STR_SIZE * sizeof(char));
                          str[0] = '\0'; 
                          $$ = str; 
                        }
    ;

%%

void yyerror(char* s){
    hasError = true;
}

int main(int argc, char *argv[]){
    if ( argc > 1 ) {
        if ( (yyin = fopen(argv[1], "r")) == 0 ) {
        perror(argv[1]);
        exit(1);
        }
    }
    int result = yyparse();
    if(result == 0 && hasError == false){
        printf("%s\n", outputString);
    }
    else{
        printf("Boom!\n");
    }
    free(outputString);
}
