#include <stdio.h>
#include <string.h>
#include "calc3.h"
#include "y.tab.h"


static int lbl;
int indexOfVarName(char* varname);
int indexOnStack(int index, int arrSize);
void storeIndexStackPos(nodeType* p);

int ex(nodeType *p) {
    int lblx, lbly, lbl1, lbl2;

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
        int symIndex = indexOfVarName(p->id.name);
        int stackIndex = indexOnStack(symIndex, 1);
        printf("\tpush\tsb[%d]\n", stackIndex); 
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
            int symIndex = indexOfVarName(p->opr.op[0]->id.name);
            int stackIndex = indexOnStack(symIndex, 1);
            printf("\tpop\tsb[%d]\n", stackIndex); 
            break;
        }
        case GETC:{
            printf("\tgetc\n");
            int symIndex = indexOfVarName(p->opr.op[0]->id.name);
            int stackIndex = indexOnStack(symIndex, 1);
            printf("\tpop\tsb[%d]\n", stackIndex); 
            break;
        }
        case GETS:{
            printf("\tgets\n");
            int symIndex = indexOfVarName(p->opr.op[0]->id.name);
            int stackIndex = indexOnStack(symIndex, 1);
            printf("\tpop\tsb[%d]\n", stackIndex); 
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
            else{
                ex(p->opr.op[0]);
                int arraySize = p->opr.op[2]->con.value;
                int symIndex = indexOfVarName(p->opr.op[1]->id.name);
                int stackIndex = indexOnStack(symIndex, arraySize); //Allocate space for the array in the stack
            }

            break;
        }
        case '=':{
            if(p->opr.nops == 2){
                ex(p->opr.op[1]);
                int symIndex = indexOfVarName(p->opr.op[0]->id.name);
                int stackIndex = indexOnStack(symIndex, 1);
                printf("\tpop\tsb[%d]\n", stackIndex); 
            }else{
                //Evaluate the rvalue of the statement
                ex(p->opr.op[2]);
                storeIndexStackPos(p); //After executing this function, the register "in" stores the index
                printf("\tpop\tsb[in]\n");//Pop the top of the stack(which stores the rvalue of the statement) to the designated position of the array

            }
            break;}
        case '[':{ //Case for accessing array
            storeIndexStackPos(p); //After executing this function, the register "in" stores the index

            //Then, push the element onto the stack
            printf("\tpush\tsb[in]\n"); 

            break;}
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
    return 0;
}
