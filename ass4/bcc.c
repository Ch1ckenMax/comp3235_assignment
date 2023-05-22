#include <stdio.h>
#include <string.h>
#include "calc3.h"
#include "y.tab.h"


static int lbl;
int indexOfVarName(char* varname);

int ex(nodeType *p) {
    int lblx, lbly, lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeConInt:       
        return p->con.value; 
        break;
    case typeConChar:
        printf("\tpush\t'%c'\n", p->con.value); 
        break;
    case typeConStr:
        printf("\tpush\t%s\n", p->conStr.str);
        break;
    case typeId:{
        int index = indexOfVarName(p->id.name);
        printf("\tpush\tsb[%d]\n", index); 
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
        case IF:{
            int e1 = ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
                if(e1){
                    ex(p->opr.op[1]);
                }
                else{
                    ex(p->opr.op[2]);
                }
            } else {
                /* if */
                if(e1){
                    ex(p->opr.op[1]);
                }
            }
            break;
        }
        case GETI:{
            printf("\tgeti\n");
            int index = indexOfVarName(p->opr.op[0]->id.name);
            printf("\tpop\tsb[%d]\n", index); 
            break;
        }
        case GETC:{
            printf("\tgetc\n");
            int index = indexOfVarName(p->opr.op[0]->id.name);
            printf("\tpop\tsb[%d]\n", index); 
            break;
        }
        case GETS:{
            printf("\tgets\n");
            int index = indexOfVarName(p->opr.op[0]->id.name);
            printf("\tpop\tsb[%d]\n", index); 
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
        case '=':{
            ex(p->opr.op[1]);
            int index = indexOfVarName(p->opr.op[0]->id.name);
            printf("\tpop\tsb[%d]\n", index); 
            break;
        }
        case UMINUS:{
            int e1 = ex(p->opr.op[0]);
            return (-e1);
            break;
        }

        case GRID_GET:{
            int x_cord = ex(p->opr.op[0]);
            int y_cord = ex(p->opr.op[1]);
            int blockType = ex(p->opr.op[2]);
            blockT[x_cord][y_cord] = blockType;
            printf("%d %d %c 0\n", x_cord, y_cord, blockType);
            break;
        }

        case GRID_ROTATE: {
            int x_cord = ex(p->opr.op[0]);
            int y_cord = ex(p->opr.op[1]);
            int rotate = ex(p->opr.op[2]);
            blockR[x_cord][y_cord] = rotate;
            printf("%d %d %c %d\n", x_cord, y_cord, blockT[x_cord][y_cord], rotate);
            break;
        }

        case GRID: {
            int xDes = ex(p->opr.op[0]);
            int yDes = ex(p->opr.op[1]);
            int xFrom = ex(p->opr.op[2]);
            int yFrom = ex(p->opr.op[3]);
            blockT[xDes][yDes] = blockT[xFrom][yFrom];
            blockR[xDes][yDes] = blockR[xFrom][yFrom];
            printf("%d %d %c %d\n", xDes, yDes, blockT[xFrom][yFrom], blockR[xFrom][yFrom]);
            break;
        }

        default:{
            int e1 = ex(p->opr.op[0]);
            int e2 = ex(p->opr.op[1]);
            switch(p->opr.oper) {
            case '+':   return e1 + e2;
            case '-':   return e1 - e2;
            case '*':   return e1 * e2;
            case '/':   return e1 / e2;
            case '%':   return e1 % e2;
            case '<':   return e1 < e2;
            case '>':   return e1 > e2;
            case GE:    return e1 >= e2;
            case LE:    return e1 <= e2;
            case NE:    return e1 != e2;
            case EQ:    return e1 == e2;
	        case AND:   return e1 && e2;
	        case OR:    return e1 || e2;
            }
        }
        }
    }
    return 0;
}
