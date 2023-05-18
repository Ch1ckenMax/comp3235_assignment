#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "calc3.h"
#include "y.tab.h"


static int lbl;
int indexOfVarName(char* varname, scopeEnum scope);
int indexOnStack(int index, int arrSize, scopeEnum scope);
int symTableAddEntry(int symIndex, char* name);
void storeIndexStackPos(nodeType* p, scopeEnum scope);
void initializeStackPos(int* stackPos, int size);
void freeSym(char** symTable, int size);

int ex(nodeType *p) {
    int lblx, lbly, lbl1, lbl2, lblFunc, lblFuncSkip;

    if (!p) return 0;
    switch(p->type) {
    case typeConInt:       
        printf("\tpush\t%d\n", p->con.value); 
        break;
    case typeConChar:
        printf("\tpush\t'%c'\n", p->con.value); 
        break;
    case typeConStr:
        printf("\tpush\t%s\n", p->conStr.str);
        break;
    case typeId:{
        /* Variable (i.e. identifier) */
        int symIndex = indexOfVarName(p->id.name, currentScope); //Finds the index of the identifier name of the variable in the current scope
        if(symIndex < 0){                                        //The above function returns -1 if the variable is not declared in the symbol table.
            symIndex = symTableAddEntry(symIndex, p->id.name);   //If variable not yet declared, add the name to the symbol table.
            numOfParams++;
        }
        int stackIndex = indexOnStack(symIndex, 1, currentScope);//Allocate space in the stack for the variable/find the position of the variable in the stack (relative to the current frame pointer)
        if(!p->id.lvalue){
            if(currentScope == global){
                printf("\tpush\tsb[%d]\n", stackIndex); 
            }
            else{
                printf("\tpush\tfp[%d]\n", stackIndex); 
            }
        }
        
        
        
        break;
    }
    case typeOpr:
        switch(p->opr.oper) {
            case FOR:
                ex(p->opr.op[0]);
                printf("L%03d:\n", lblx = lbl++);
                ex(p->opr.op[1]);
                printf("\tj0\tL%03d\n", lbly = lbl++);
                ex(p->opr.op[3]);
                ex(p->opr.op[2]);
                printf("\tjmp\tL%03d\n", lblx);
                printf("L%03d:\n", lbly);
                break;
            case WHILE:
                printf("L%03d:\n", lbl1 = lbl++);
                ex(p->opr.op[0]);
                printf("\tj0\tL%03d\n", lbl2 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl1);
                printf("L%03d:\n", lbl2);
                break;
            case IF:
                ex(p->opr.op[0]);
                if (p->opr.nops > 2) {
                    /* if else */
                    printf("\tj0\tL%03d\n", lbl1 = lbl++);
                    ex(p->opr.op[1]);
                    printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                    printf("L%03d:\n", lbl1);
                    ex(p->opr.op[2]);
                    printf("L%03d:\n", lbl2);
                } else {
                    /* if */
                    printf("\tj0\tL%03d\n", lbl1 = lbl++);
                    ex(p->opr.op[1]);
                    printf("L%03d:\n", lbl1);
                }
                break;
            case GETI:{
                printf("\tgeti\n");
                int symIndex = indexOfVarName(p->opr.op[0]->id.name, currentScope);
                if(symIndex < 0){
                    symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                }
                int stackIndex = indexOnStack(symIndex, 1, currentScope);
                if(currentScope == global){
                    printf("\tpop\tsb[%d]\n", stackIndex); 
                }
                else{
                    printf("\tpop\tfp[%d]\n", stackIndex); 
                }
                break;
            }
            case GETC:{
                printf("\tgetc\n");
                int symIndex = indexOfVarName(p->opr.op[0]->id.name, currentScope);
                if(symIndex < 0){
                    symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                }
                int stackIndex = indexOnStack(symIndex, 1, currentScope);
                if(currentScope == global){
                    printf("\tpop\tsb[%d]\n", stackIndex); 
                }
                else{
                    printf("\tpop\tfp[%d]\n", stackIndex); 
                } 
                break;
            }
            case GETS:{
                printf("\tgets\n");
                int symIndex = indexOfVarName(p->opr.op[0]->id.name, currentScope);
                if(symIndex < 0){
                    symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                    
                }
                int stackIndex = indexOnStack(symIndex, 1, currentScope);
                if(currentScope == global){
                    printf("\tpop\tsb[%d]\n", stackIndex); 
                }
                else{
                    printf("\tpop\tfp[%d]\n", stackIndex); 
                } 
                break;
            }
            case PUTI:
                ex(p->opr.op[0]);
                printf("\tputi\n");
                break;
            case PUTC:
                ex(p->opr.op[0]);
                printf("\tputc\n");
                break;
            case PUTS:
                ex(p->opr.op[0]);
                printf("\tputs\n");
                break;
            case PUTI_:
                ex(p->opr.op[0]);
                printf("\tputi_\n");
                break;
            case PUTC_:
                ex(p->opr.op[0]);
                printf("\tputc_\n");
                break;
            case PUTS_:
                ex(p->opr.op[0]);
                printf("\tputs_\n");
                break;
            case ARRAY:{
                if (p->opr.nops == 1){
                    ex(p->opr.op[0]);
                }
                else if (p->opr.nops == 3){
                    ex(p->opr.op[0]);
                    int arraySize = p->opr.op[2]->con.value;
                    int symIndex = indexOfVarName(p->opr.op[1]->id.name, currentScope);
                    if(symIndex < 0){
                        symIndex = symTableAddEntry(symIndex, p->opr.op[1]->id.name);
                    }
                    int stackIndex = indexOnStack(symIndex, arraySize, currentScope); //Allocate space for the array in the stack
                }
                else{
                    ex(p->opr.op[0]);
                    int arraySize = p->opr.op[2]->con.value;
                    int symIndex = indexOfVarName(p->opr.op[1]->id.name, currentScope);
                    if(symIndex < 0){
                        symIndex = symTableAddEntry(symIndex, p->opr.op[1]->id.name);
                    }
                    int stackIndex = indexOnStack(symIndex, arraySize, currentScope); //Allocate space for the array in the stack

                    ex(p->opr.op[3]);
                    printf("\tpop\tac\n"); //Save the value to ac
                    for(int i = stackIndex; i < stackIndex + arraySize; i++){
                        printf("\tpush\tac\n");
                        if(currentScope == global){
                            printf("\tpop\tsb[%d]\n", i);
                        }
                        else{
                            printf("\tpop\tfp[%d]\n", i);
                        }
                    }
                }

                break;
            }
            case '=':{
                if(p->opr.nops == 2){
                    ex(p->opr.op[1]);
                    int symIndex = indexOfVarName(p->opr.op[0]->id.name, currentScope);
                    if(symIndex < 0){
                        symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                    }
                    int stackIndex = indexOnStack(symIndex, 1, currentScope);
                    if(currentScope == global){
                        printf("\tpop\tsb[%d]\n", stackIndex); 
                    }
                    else{
                        printf("\tpop\tfp[%d]\n", stackIndex); 
                    }
                }else{
                    //Evaluate the rvalue of the statement
                    ex(p->opr.op[2]);
                    storeIndexStackPos(p, currentScope); //After executing this function, the register "in" stores the index
                    if(currentScope == global){
                        printf("\tpop\tsb[in]\n");//Pop the top of the stack(which stores the rvalue of the statement) to the designated position of the array
                    }else{
                        printf("\tpop\tfp[in]\n");
                    }

                }
                break;
            }
            case FUNC: {
                
                //Variables for "backpatching"
                int std_out = dup(STDOUT_FILENO); //Create a copy of file descriptor of stdout
                int fds[2]; //For storing file descriptors of a pipe in linux

                nextStackPosLocal = 0;
                nextSymIndexLocal = 0;
                numOfParams = 0;
                hasReturn = false;
                initializeStackPos(symStackPosLocal, STACK_RESERVE_SIZE);
                lblFunc = lbl++; //Jump position for this function

                //Add function name to symbol table
                int symIndex = indexOfVarName(p->opr.op[0]->id.name, global);
                if(symIndex < 0){
                    symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                }
                symStackPos[symIndex] = lblFunc;
                currentScope = local;

                //Flush stdout before proceeding. Anything before this point is guaranteed to be flushed to stdout
                fflush(stdout);

                //Create a pipe. This is a single threaded program but I am still going to use pipe anyway
                //I want to do "backpatching" by redirecting stdout to a pipe, then retrieve the content from pipe later
                if(pipe(fds) == -1){
                    fprintf(stderr, "Error occured when creating a pipe for backpatching in function declarations.\n");
                }

                //Replace the stdout to pipe write end
                if(dup2(fds[1], STDOUT_FILENO) == -1){
                    fprintf(stderr, "Error occured when creating a pipe for backpatching in function declarations.\n");
                }

                //Parameter list
                ex(p->opr.op[1]);

                //Generate code for function body
                ex(p->opr.op[2]);
                if(!hasReturn){
                    //If there is no return statement in the function body, add one to the function body.
                    printf("\tpush\t0\n"); //Placeholder
                    printf("\tRET\n");
                }
                printf("L%03d:\n", lblFuncSkip = lbl++);
                fflush(stdout); //Flush it so the printf stuff is flushed into the pipe stream
                close(fds[1]);

                //Restore stdout
                dup2(std_out, STDOUT_FILENO);
                close(std_out);

                ssize_t n = read(fds[0], buf, BUFSIZ); //Read from the pipe
                buf[n] = '\0';

                close(fds[0]);

                //JUMP POSITION. VAR <number of parameters>, <local variables created>
                printf("\tjmp\tL%03d\n", lblFuncSkip);
                //printf("wot?%d\n", nextStackPosLocal);
                printf("L%03d:\n\tvar\t%d,%d\n", lblFunc, numOfParams, nextStackPosLocal - numOfParams);
                printf("%s", buf);

                currentScope = global;
                freeSym(symLocal, nextSymIndexLocal);

                break;
            }
            case RET: {
                ex(p->opr.op[0]);
                printf("\tRET\n");
                hasReturn = true;
                break;
            }
            case '(': {
                ex(p->opr.op[1]);
                int symIndex = indexOfVarName(p->opr.op[0]->id.name, global);
                printf("\tCALL\tL%03d\n", symStackPos[symIndex]);
                break;
            }
            case PARAM_LIST:{
                ex(p->opr.op[0]);
                ex(p->opr.op[1]);
                break;
            }
            case ARG_LIST:{
                ex(p->opr.op[0]);
                ex(p->opr.op[1]);
                break;
            }
            case '[':{ //Case for accessing array
                storeIndexStackPos(p, currentScope); //After executing this function, the register "in" stores the index

                //Then, push the element onto the stack
                if(currentScope == global){
                    printf("\tpush\tsb[in]\n"); 
                }
                else{
                    printf("\tpush\tfp[in]\n"); 
                }

                break;
            }
            case ']':{
                storeIndexStackPos(p, global); //After executing this function, the register "in" stores the index

                //Then, push the element onto the stack
                printf("\tpush\tsb[in]\n"); 

                break;
            }
            case '@':{ //Global variable
                if(p->opr.nops == 1){
                    int savedScope = currentScope; //Save the current scope
                    currentScope = global; //As the global variable is needed, change the current scope to global
                    ex(p->opr.op[0]); //execute the variable
                    currentScope = savedScope; //restore the current scope
                }
                else if(p->opr.nops == 2){
                    ex(p->opr.op[1]);
                    int symIndex = indexOfVarName(p->opr.op[0]->id.name, global);
                    if(symIndex < 0){
                        symIndex = symTableAddEntry(symIndex, p->opr.op[0]->id.name);
                    }
                    int stackIndex = indexOnStack(symIndex, 1, global);
                    printf("\tpop\tsb[%d]\n", stackIndex); 
                }else{
                    ex(p->opr.op[2]);
                    storeIndexStackPos(p, global); //After executing this function, the register "in" stores the index
                    printf("\tpop\tsb[in]\n");
                }
                
                break;
            }
            case UMINUS:    
                ex(p->opr.op[0]);
                printf("\tneg\n");
                break;
            default:
                ex(p->opr.op[0]);
                ex(p->opr.op[1]);
                switch(p->opr.oper) {
                    case '+':   printf("\tadd\n"); break;
                    case '-':   printf("\tsub\n"); break; 
                    case '*':   printf("\tmul\n"); break;
                    case '/':   printf("\tdiv\n"); break;
                    case '%':   printf("\tmod\n"); break;
                    case '<':   printf("\tcomplt\n"); break;
                    case '>':   printf("\tcompgt\n"); break;
                    case GE:    printf("\tcompge\n"); break;
                    case LE:    printf("\tcomple\n"); break;
                    case NE:    printf("\tcompne\n"); break;
                    case EQ:    printf("\tcompeq\n"); break;
                    case AND:   printf("\tand\n"); break;
                    case OR:    printf("\tor\n"); break;
                }
        }
    }
}

//Add an entry to the sym table
int symTableAddEntry(int symIndex, char* name){
    if(currentScope == global){
        //printf("Declare %s globally\n", name);
        sym[nextSymIndex] = strdup(name);
        return nextSymIndex++;
    }
    else{
        //printf("Declare %s locally\n", name);
        symLocal[nextSymIndexLocal] = strdup(name);
        return nextSymIndexLocal++;
    }
}

//Given a variable name, find its index number in the symbol table.
int indexOfVarName(char* varname, scopeEnum scope){
    bool indexFound = false;
    int index = 0;
    if(scope == global){
        while(indexFound == false && index < nextSymIndex){
            if(strcmp(sym[index], varname) == 0){
                indexFound = true;
            }
            else{
                index++;
            }
        }
    }else{
        while(indexFound == false && index < nextSymIndexLocal){
            if(strcmp(symLocal[index], varname) == 0){
                indexFound = true;
            }
            else{
                index++;
            }
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
int indexOnStack(int index, int size, scopeEnum scope){
    if(scope == global){
        if(symStackPos[index] == STACKPOS_NOT_YET_INIT){ //Space not yet reserved
            symStackPos[index] = nextStackPos;
            nextStackPos += size;
        }
        return symStackPos[index];
    }
    else{
        if(symStackPosLocal[index] == STACKPOS_NOT_YET_INIT){ //Space not yet reserved
            symStackPosLocal[index] = nextStackPosLocal;
            nextStackPosLocal += size;
        }
        return symStackPosLocal[index];
    }
    
}

//Given a nodeType pointer that points to a nodeType object, with the first operator being a VARIABLE and the second operator being an expr
//Store the position of the variable in the stack to the reigster "in" (without the offset of fp or sb)
void storeIndexStackPos(nodeType* p, scopeEnum scope){
    //Find start position of the array
    int symIndex = indexOfVarName(p->opr.op[0]->id.name, scope);
    int stackIndex = indexOnStack(symIndex, 1, scope);

    //Generate code to evaluate the expression for the array index
    ex(p->opr.op[1]);
    //Position of the start of the array            
    printf("\tpush\t%d\n", stackIndex); 
    //Add them together to get the address for the element (without the offset of fp or sb)
    printf("\tadd\n");

    //Store the adress to "in"
    printf("\tpop\tin\n");
    
}