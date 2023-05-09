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
int indexOnStack(int index, int arrSize);

void yyerror(char *s);
char* sym[SYM_SIZE];                    /* symbol table */
int symStackPos[STACK_RESERVE_SIZE];
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
%nonassoc IFX
%nonassoc ELSE
%token GETI GETC GETS PUTI PUTC PUTS PUTI_ PUTC_ PUTS_
%token ARRAY
%token <sPtr> VARIABLE

%left AND OR

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list arr_decl_list

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
        | VARIABLE '[' expr ']' '=' expr ';' { $$ = opr('=', 3, id($1), $3, $6);}
        | ARRAY arr_decl_list ';' { $$ = opr(ARRAY, 1, $2); }
	    | FOR '(' stmt stmt stmt ')' stmt { $$ = opr(FOR, 4, $3, $4,
$5, $7); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2; }
        ;

arr_decl_list:
          VARIABLE '[' INTEGER ']'                  { $$ = opr(ARRAY, 3, NULL, id($1), conInt($3)); }
        | arr_decl_list ',' VARIABLE '[' INTEGER ']'    { $$ = opr(ARRAY, 3, $1, id($3), conInt($5)); }

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = conInt($1); }
        | CHARACTER             { $$ = conChar($1); }
        | STRING                { $$ = conStr($1); }
        | VARIABLE              { $$ = id($1); }
        | VARIABLE '[' expr ']' { $$ = opr('[', 2, id($1), $3); }
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

//This function does stuff on the symbol table
nodeType *id(char* name) {
    nodeType *p;
    size_t nodeSize;

    if(nextSymIndex >= SYM_SIZE){ /* symbol table is full */
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
        sym[nextSymIndex] = strdup(name);
        nextSymIndex++;
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
    //printf("Sym: %s, Pos: %d\n", varname, index);
    if(indexFound == true){
        return index;
    }
    else{
        return -1;
    }
}

//Given the index of the variable on the symbol table, find the position of the variable on the stack
//If space is not yet reserved in the stack, reserve space.
int indexOnStack(int index, int size){
    if(symStackPos[index] == STACKPOS_NOT_YET_INIT){ //Space not yet reserved
        symStackPos[index] = nextStackPos;
        nextStackPos += size;
    }
    //printf("Stack for: %d, stack pos: %d\n", index, symStackPos[index]);
    return symStackPos[index];
}

//Given a nodeType pointer that points to a nodeType object, with the first operator being a VARIABLE and the second operator being an expr
//Store the position of the variable in the stack to the reigster "in" (without the offset of fp or sb)
void storeIndexStackPos(nodeType* p){
    //Find start position of the array
    int symIndex = indexOfVarName(p->opr.op[0]->id.name);
    int stackIndex = indexOnStack(symIndex, 1);


    //Generate code to evaluate the expression for the array index
    ex(p->opr.op[1]);
    //Position of the start of the array            
    printf("\tpush\t%d\n", stackIndex); 
    //Add them together to get the address for the element (without the offset of fp or sb)
    printf("\tadd\n");

    //Store the adress to "in"
    printf("\tpop\tin\n");
    
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv) {
    extern FILE* yyin;

    //Initialize the indexOnStack array
    for(int i = 0; i < STACK_RESERVE_SIZE; i++){
        symStackPos[i] = STACKPOS_NOT_YET_INIT;
    }

    yyin = fopen(argv[1], "r");

    printf("\tpush\t%d\n", STACK_RESERVE_SIZE); printf("\tpop\tsp\n"); //Reserve space in the stack for variables

    yyparse();
    //Free the character arrays in the symbol table
    for(int i = 0; i < nextSymIndex; i++){
        free(sym[i]);
    }
    return 0;
}
