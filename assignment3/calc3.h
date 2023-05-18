#include <stdbool.h>
#define SYM_SIZE 50
#define STACK_RESERVE_SIZE 1000
#define STACKPOS_NOT_YET_INIT -9999

typedef enum { typeConInt, typeId, typeOpr, typeConChar, typeConStr } nodeEnum;
typedef enum { global, local } scopeEnum;

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

typedef struct {
    char* str;                  /* pointer to a string */
} conStrNodeType;           

/* identifiers */
typedef struct {
    char* name;                      /* subscript to sym array */
    bool lvalue;                     /* states whether the identifier is used as lvalue or not */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];  /* operands (expandable) */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    /* union must be last entry in nodeType */
    /* because operNodeType may dynamically increase */
    union {
        conNodeType con;        /* constants */
        conStrNodeType conStr;  /* string constant */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

/* -- For Global Scope -- */
extern char* sym[SYM_SIZE];
static int nextSymIndex = 0; //Index of the next element to be filled in the symbol table
extern int symStackPos[STACK_RESERVE_SIZE]; //position of the variable in the stack - sb
static int nextStackPos = 0;

/* -- For Local Scope -- */
extern char* symLocal[SYM_SIZE];
static int nextSymIndexLocal = 0;
extern int symStackPosLocal[STACK_RESERVE_SIZE]; // relative to fp
static int nextStackPosLocal = 0;
char buf[BUFSIZ]; //For "backpatching" using pipe

/* -- Static Variables for "Inherited Attributes" -- */
static int numOfParams = 0; //For counting the number of parameters
static bool hasReturn = 0; //For checking if the fucntion body has a return statement
static scopeEnum currentScope = global;
