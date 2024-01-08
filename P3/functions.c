#include "functions.h"

char **quads;
int next_quad;
int next_temp;

estructura calculate(estructura operando1, estructura operator, estructura operando2) {
    float op1 = (operando1.tipo == INT) ? operando1.integer : operando1.real;
    float op2 = (operando2.tipo == INT) ? operando2.integer : operando2.real;
    float res;	
    estructura result;

    result.tipo = (operando1.tipo == INT && operando2.tipo == INT) ? INT : FLOAT;
    
    char op = (operator.string)[0];
    switch(op) {
        case '+':
            res = op1 + op2;
            sprintf(operator.string, result.tipo == INT ? " ADDI " : " ADDF ");
            break;
        case '-':
            res = op1 - op2;
            sprintf(operator.string, result.tipo == INT ? " SUBI " : " SUBF ");
            break;
        case '*':
            res = op1 * op2;
            sprintf(operator.string, result.tipo == INT ? " MULI " : " MULF ");
            break;
        case '/':
            res = op1 / op2;
            sprintf(operator.string, result.tipo == INT ? " DIVI " : " DIVF ");
            break;
        case '%':
            if (result.tipo == INT) {
                res = (int)op1 % (int)op2;
                strcpy(operator.string, " MODI ");
            } else {
                yyerror("ERROR SEMANTICO: Modulo solo admite enteros");
            }
            break;
        case '^':
            res = pow(op1, op2);
            break;
        default:
            yyerror("ERROR: Operación desconocida");
    }

    if (result.tipo == INT) result.integer = res;
    else result.real = res;
    
    return set_valor(result);
}


estructura set_valor(estructura r) {
    char* aux;

    if (r.tipo == INT)
     {
    	aux = malloc(sizeof(int));
        sprintf(aux, "%d", r.integer);
    } else {
    	aux = malloc(sizeof(float));
        sprintf(aux, "%f", r.real);
    }
    r.value = aux;
    return r;
}


estructura negate(estructura value) {
    if(value.tipo == INT) 	value.integer = -value.integer;  sprintf(value.string, " CHSI ");
    if(value.tipo == FLOAT) 	value.real = -value.real;  sprintf(value.string, " CHSF ");
    return value;
}


void generate(int nargs, ...) {
    char *quad = malloc(100);
    if (!quad) {
        yyerror("Error de memoria");
        return;
    }
    strcpy(quad, "");
    char *aux;
    va_list ap;
    va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) {
        aux = va_arg(ap, char*);
        strcat(quad, aux);
    }
    va_end(ap);
    if (!quads) {
        yyerror("Error de memoria");
        free(quad);
        return;
    }
    quads[next_quad-1] = malloc(50);
    sprintf(quads[next_quad-1], "%d: %s", next_quad, quad);
    quads = realloc(quads, 50 * (next_quad + 1));
    next_quad++;
}

char* temporal(){
    char* var = malloc(6);
    char* numero = malloc(3);
    sprintf(numero, "%d", next_temp);
    strcpy(var, "$t0");
    strcat(var, numero);
    next_temp++;
    return var;
}


void put(estructura r) {
    generate(2, "PARAM ", r.value);
    if (r.tipo == INT) 		generate(1, "CALL PUTI, 1");
    if (r.tipo == FLOAT) 	generate(1, "CALL PUTF, 1");
}


//PRACTICA 3 FUNCTIONS 

quad* crearLista(int i) {
    quad* lista = malloc(sizeof(quad));
    if (!lista) {
        yyerror("Error de memoria");
        return NULL;
    }
    lista->num_quad = i;
    lista->next_quad = NULL;
    return lista;
}

quad* combine(quad *p1, quad *p2) {
    if (p1 == NULL) return p2;
    quad *aux = p1;
    while (aux->next_quad != NULL) {
        aux = aux->next_quad;
    }
    aux->next_quad = p2;
    return p1;
}

void complete(quad *p, int i) {
    while (p != NULL) {
        if (quads[p->num_quad - 1]) {
            char* aux = malloc(strlen(quads[p->num_quad - 1]) + 20); // allocate space for modification
            if (!aux) {
                yyerror("Error de memoria");
                return;
            }
            snprintf(aux, strlen(quads[p->num_quad - 1]) + 20, "%s %d", quads[p->num_quad - 1], i);
            free(quads[p->num_quad - 1]); // free old quad string
            quads[p->num_quad - 1] = aux; // update with new string
        }
        p = p->next_quad;
    }
}


estructura boolean_calculate(estructura operando1, estructura operador, estructura operando2) {
    char *op = operador.string;
    estructura result;
    result.tipo = BOOLEAN;
    float r1, r2;
    if (operando1.tipo == INT) 	r1 = operando1.integer;
    if (operando1.tipo == FLOAT) 	r1 = operando1.real;
    if (operando2.tipo == INT) 	r2 = operando2.integer;
    if (operando2.tipo == FLOAT) 	r2 = operando2.real;

    char* aux = malloc(7);
    if (strlen(op) == 1) {
        if(op[0] == '>') { result.boolean = r1 > r2; sprintf(aux, " GT"); }
        if(op[0] == '<') { result.boolean = r1 < r2; sprintf(aux, " LT"); }
	if(op[0] == '=') { result.boolean = r1 < r2; sprintf(aux, " EQ"); }

    } 
    else {
        if(op[0] == '>' && op[1] == '=') { result.boolean = r1 >= r2; sprintf(aux, " GE"); }
        if(op[0] == '<' && op[1] == '=') { result.boolean = r1 <= r2; sprintf(aux, " LE"); }
        if(op[0] == '<' && op[1] == '>') { result.boolean = r1 != r2; sprintf(aux, " NE"); }
    }
    if (operando1.tipo == INT && operando2.tipo == INT) strcat(aux, "I ");
    else strcat(aux, "F ");
    
    strcpy(operador.string, aux);
    return set_valor(result);
}

