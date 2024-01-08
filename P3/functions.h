#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include "types.h"
#include "symtab.h"


estructura calculate(estructura operando1, estructura operator, estructura operando2);
estructura set_valor(estructura r);
estructura negate(estructura value);
estructura boolean_calculate(estructura operando1, estructura operador, estructura operando2);
void yyerror(char *err);
void generate(int nargs, ...);
void complete(quad *p, int i);
char* temporal();
void put(estructura r);
quad* crearLista(int i);
quad* combine(quad *p1, quad *p2);
#endif // FUNCTIONS_H

