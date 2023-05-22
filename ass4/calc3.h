#include <stdbool.h>
#define SYM_SIZE 26

typedef enum { typeConInt, typeId, typeOpr, typeConChar, typeConStr } nodeEnum;

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

static int blockT[8][8] = {0};
static int blockR[8][8] = {0};

static int varStack[SYM_SIZE];
extern char* sym[SYM_SIZE];
static int nextVarIndex = 0; //Index of the next element to be filled in the symbol table

