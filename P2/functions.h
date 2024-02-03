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
void declare_array(char* arrayName, int elements);
void assign_array(const char* arrayName, int index, char* value);
void access_array(const char* arrayName, int index);
estructura convert_to_int(estructura var);
estructura convert_to_float(estructura var);
estructura calculate(estructura operando1, estructura operador, estructura operando2);
estructura negate(estructura valor);
void yyerror(char *err);
estructura set_valor(estructura r);
void generate(int nargs, ...);
char* temporal();
void put(estructura r);
#endif // FUNCTIONS_H

