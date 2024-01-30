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
estructura strlen_function(estructura str);
void print_result(estructura s);

#endif // FUNCTIONS_H

