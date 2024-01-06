#include "functions.h"

char **quads;
int next_quad;
int next_temp;

// Modified calculate function
estructura calculate(estructura operando1, estructura operator, estructura operando2) {
    float op1,op2;
    if (operando1.tipo == INT) {op1 = operando1.integer;}
    if (operando1.tipo == FLOAT) {op1 = operando1.real;}
    if (operando2.tipo == INT) {op2 = operando2.integer;}
    if (operando2.tipo == FLOAT) {op2 = operando2.real;}
    float res;	
    estructura result;
    char op=(operator.string)[0];
    if (operando1.tipo == INT && operando2.tipo == INT) {
    	result.tipo = INT;
    	if (op == '+') sprintf(operator.string, " ADDI ");
    	if (op == '-') sprintf(operator.string, " SUBI ");
    	if (op == '*') sprintf(operator.string, " MULI ");
    	if (op == '/') sprintf(operator.string, " DIVI ");
	if (op == '%') sprintf(operator.string, " MODI ");
    }
    else  {
    	result.tipo = FLOAT;
    	if (op == '+') sprintf(operator.string, " ADDF ");
    	if (op == '-') sprintf(operator.string, " SUBF ");
    	if (op == '*') sprintf(operator.string, " MULF ");
    	if (op == '/') sprintf(operator.string, " DIVF ");
    }

    if (op == '+') res = op1 + op2;
    if (op == '-') res = op1 - op2;
    if (op == '*') res = op1 * op2;
    if (op == '/') res = op1 / op2;
    if (op == '^') res = pow(op1, op2);

    if (op == '%') {
    	if (operando1.tipo == INT && operando2.tipo == INT) res = (int)op1 % (int)op2;
    	else yyerror("ERROR SEMANTICO: Modulo solo admite enteros");
	}
    if (operando1.tipo == INT && operando2.tipo == INT) result.integer = res;
    else  result.real = res;
    
    return set_valor(result);
}

// Modified set_valor function
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

// Modified generate function
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


