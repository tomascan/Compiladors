#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "types.h"
#include "symtab.h"


// Declaraciones de las funciones
estructura calculate(estructura operando1, estructura operador, estructura operando2);
estructura negate(estructura valor);
void yyerror(char *err);
estructura set_valor(estructura r);
void generate(int nargs, ...);
char* temporal();
void put(estructura r);
#endif // FUNCTIONS_H

