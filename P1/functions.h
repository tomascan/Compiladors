#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "types.h"
#include "symtab.h"


extern char* modo_formato;
void yyerror(const char *err);

// Declaraciones de las funciones
estructura sumar(estructura op1, estructura op2);
estructura restar(estructura op1, estructura op2);
estructura multiplicar(estructura op1, estructura op2);
estructura dividir(estructura op1, estructura op2);
estructura potencia(estructura op1, estructura op2);
estructura concatenar(estructura op1, estructura op2);
estructura calculate(estructura op1, estructura operador, estructura op2);
estructura trigonometric_op(estructura value, estructura op);
estructura check_boolean(estructura op1, estructura operador, estructura op2);
estructura negate(estructura value);
void cambiar_modo_formato(const char* modo);
void initializeArray(const char* arrayName, int size);
void assignArrayElement(const char* arrayName, int index, estructura value);
void printArray(const char* arrayName);
int accessArrayElement(const char* arrayName, int index);
void print_result(estructura s);

#endif // FUNCTIONS_H

