#include "functions.h"

char **quads;
int next_quad;
int next_temp;



estructura convert_to_int(estructura var) {
    estructura result;
    // Assuming sym_lookup and sym_enter are correctly implemented to manage the symbol table
    if (sym_lookup(var.string, &result) == SYMTAB_OK) {
        // Perform the conversion
        result.tipo = INT;
        result.integer = (int)result.real;
        //Update the 'value' field to reflect the new integer value
        char buffer[sizeof(int)]; // Ensure the buffer is large enough
        sprintf(buffer, "%d", result.integer);
        free(result.value); // Free the old value string if necessary
        result.value = strdup(buffer); // Update with the new value string
        // Update the symbol table entry
        sym_enter(var.string, &result);
    }
    return result;
}


estructura convert_to_float(estructura var) {
    estructura result;
    // Assuming sym_lookup and sym_enter are correctly implemented to manage the symbol table
    if (sym_lookup(var.string, &result) == SYMTAB_OK) {
        // Perform the conversion
        result.tipo = FLOAT;
        result.real = (float)result.integer;
        // Update the 'value' field to reflect the new integer value
        char buffer[20]; // Ensure the buffer is large enough
        sprintf(buffer, "%.2f", result.real);
        free(result.value);
        result.value = strdup(buffer); // Update with the new value string
        // Update the symbol table entry
        sym_enter(var.string, &result);
    }
    return result;
}



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


void declare_array(char* arrayName, int elements) {
    estructura array;
    array.tipo = ARRAY;
    int elementSize = sizeof(int); //El indice siempre va a ser entero
    int totalSize = elements * elementSize;
    array.arraySize = elements;
    sym_enter(arrayName, &array);
    char sizeStr[20];
    sprintf(sizeStr, "%d", totalSize);
    generate(4, arrayName, " := ", "ALLOC ", sizeStr);
//    for(int i = 0; i < elements; ++i) {
//        char offsetStr[20];
//        sprintf(offsetStr, "[%d]", i * elementSize);
//        generate(5, arrayName, offsetStr, " := ", "0");
//    }
}

void assign_array(const char* arrayName, int index, estructura r) {
    estructura array_info;
    if (sym_lookup(arrayName, &array_info) == SYMTAB_OK && array_info.tipo == ARRAY) {
        int offset = sizeof(r.tipo) * index; // Tamaño del tipo de elemento en el array
        char* aux = malloc(sizeof(r.tipo) * index);
        sprintf(aux, "%d", offset);
        // Genera un nombre de temporal y guarda el desplazamiento calculado en él
        char* tempOffset = temporal();
        generate(5, tempOffset, " := ", "i ", "MULI ", aux); // Asume 'i' representa el índice
        // Usa el temporal para la asignación al elemento del array
        generate(5, arrayName, "[", tempOffset, "] := ", r.value);
    } else {
        yyerror("Identificador no encontrado o no es un array");
    }
}



void generate(int nargs, ...) {
    va_list ap;
    va_start(ap, nargs);
    
    int size = 0;
    for (int i = 0; i < nargs; i++) {
        size += strlen(va_arg(ap, char*));
    }
    va_end(ap);

    // Allocate memory for the quad with the exact needed size
    char *quad = malloc(size + 1); // +1 for the null terminator
    if (!quad) {
        yyerror("Error de memoria");
        return;
    }
    quad[0] = '\0';

    va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) {
        strcat(quad, va_arg(ap, char*));
    }
    va_end(ap);

    // Allocate and format the entry in the quads array
    quads[next_quad-1] = malloc(strlen(quad));
    sprintf(quads[next_quad-1], "%d: %s", next_quad, quad);
    free(quad); // Free the temporary quad buffer

    // Resize quads array for the next quad
    quads = realloc(quads, (next_quad + 1) * sizeof(char*));
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


