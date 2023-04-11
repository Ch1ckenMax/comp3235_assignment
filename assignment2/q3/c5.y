%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "calc3.h"



/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* name);
nodeType *conInt(int value);
nodeType *conChar(int value);
nodeType *conStr(char* str);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
int indexOfVarName(char* varname);

void yyerror(char *s);
char* sym[SYM_SIZE];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char* sPtr;                 /* string pointer */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <iValue> CHARACTER
%token <sPtr> STRING
%token FOR WHILE IF
%token GETI GETC GETS PUTI PUTC PUTS PUTI_ PUTC_ PUTS_
%nonassoc IFX
%nonassoc ELSE
%token <sPtr> VARIABLE

%left AND OR

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* NULL */
        ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                       { $$ = $1; }
        | PUTI '(' expr ')' ';'         { $$ = opr(PUTI, 1, $3); }
        | PUTC '(' expr ')' ';'         { $$ = opr(PUTC, 1, $3); }
        | PUTS '(' expr ')' ';'         { $$ = opr(PUTS, 1, $3); }
        | PUTI_ '(' expr ')' ';'         { $$ = opr(PUTI_, 1, $3); }
        | PUTC_ '(' expr ')' ';'         { $$ = opr(PUTC_, 1, $3); }
        | PUTS_ '(' expr ')' ';'         { $$ = opr(PUTS_, 1, $3); }
	    | GETI '(' VARIABLE ')' ';'		 { $$ = opr(GETI, 1, id($3)); }
        | GETC '(' VARIABLE ')' ';'		 { $$ = opr(GETC, 1, id($3)); }
        | GETS '(' VARIABLE ')' ';'		 { $$ = opr(GETS, 1, id($3)); }
        | VARIABLE '=' expr ';'          { $$ = opr('=', 2, id($1), $3);}
	| FOR '(' stmt stmt stmt ')' stmt { $$ = opr(FOR, 4, $3, $4,
$5, $7); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2; }
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = conInt($1); }
        | CHARACTER             { $$ = conChar($1); }
        | STRING                { $$ = conStr($1); }
        | VARIABLE              { $$ = id($1); }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '%' expr         { $$ = opr('%', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
	| expr AND expr		{ $$ = opr(AND, 2, $1, $3); }
	| expr OR expr		{ $$ = opr(OR, 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)

nodeType *conInt(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeConInt;
    p->con.value = value;

    return p;
}

nodeType *conChar(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeConChar;
    p->con.value = value;

    return p;
}

nodeType *conStr(char* str) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeConStr;
    p->conStr.str = str;

    return p;
}

nodeType *id(char* name) {
    nodeType *p;
    size_t nodeSize;

    if(nextVarIndex >= SYM_SIZE){ /* symbol table is full */
        yyerror("too many variables declared");
    }

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");


    /* copy information */
    p->type = typeId;
    p->id.name = name;

    /* check if the variable is already declared */
    int index = indexOfVarName(name);
    if(index < 0){
        /* put the variable to the symbol table */
        sym[nextVarIndex] = strdup(name);
        nextVarIndex++;
    }


    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    size_t nodeSize;
    int i;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) +
        (nops - 1) * sizeof(nodeType*);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    if (p->type == typeConStr) { /* free the character array */
        free(p->conStr.str);
    }
    if (p->type == typeId) { /* free the character array */
        free(p->id.name);
    }
    free (p);
}


//Given a variable name, find its index number in the symbol table.
//If not found, return -1
int indexOfVarName(char* varname){
    bool indexFound = false;
    int index = 0;
    while(indexFound == false && sym[index] != NULL){
        if(strcmp(sym[index], varname) == 0){
            indexFound = true;
        }
        else{
            index++;
        }
    }
    if(indexFound == true){
        return index;
    }
    else{
        return -1;
    }
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv) {
extern FILE* yyin;
    yyin = fopen(argv[1], "r");

    printf("\tpush\t%d\n", SYM_SIZE); printf("\tpop\tsp\n"); //Reserve space in the stack for variables

    yyparse();
    //Free the character arrays in the symbol table
    for(int i = 0; i < nextVarIndex; i++){
        free(sym[i]);
    }
    return 0;
}
