#include "functions.h"
#include <math.h>

#define PI 3.14159265358979323846
#define E 2.7818281828459045

// Funciones auxiliares para la conversión de tipos
float getFloatValue(estructura op) {
    return (op.type == INT) ? (float)op.integer : op.real;
}


// Operaciones aritméticas
// SUMA 
estructura sumar(estructura op1, estructura op2) {
    estructura result;
    result.type = (op1.type == FLOAT || op2.type == FLOAT) ? FLOAT : INT;
    
    if (result.type == FLOAT) {
        result.real = getFloatValue(op1) + getFloatValue(op2);
    } else {
        result.integer = op1.integer + op2.integer;
    }
    
    return result;
}

//RESTA
estructura restar(estructura op1, estructura op2) {
    estructura result;
    result.type = (op1.type == FLOAT || op2.type == FLOAT) ? FLOAT : INT;
    
    if (result.type == FLOAT) {
        result.real = getFloatValue(op1) - getFloatValue(op2);
    } else {
        result.integer = op1.integer - op2.integer;
    }
    
    return result;
}

//MULTIPLICACION
estructura multiplicar(estructura op1, estructura op2) {
    estructura result;
    result.type = (op1.type == FLOAT || op2.type == FLOAT) ? FLOAT : INT;
    
    if (result.type == FLOAT) {
        result.real = getFloatValue(op1) * getFloatValue(op2);
    } else {
        result.integer = op1.integer * op2.integer;
    }
    
    return result;
}

//DIVISION
estructura dividir(estructura op1, estructura op2) {
    estructura result;
    result.type = FLOAT;

    float valor1 = getFloatValue(op1);
    float valor2 = getFloatValue(op2);

    if (valor2 == 0.0f) {
        fprintf(stderr, "Error: División por cero.\n");
    }

    result.real = valor1 / valor2;

    return result;
}


//MODULO
estructura modulo(estructura op1, estructura op2) {
    estructura result;
    result.type = INT;

    // Asegúrate de que ambas estructuras sean de tipo INT
    if (op1.type == INT && op2.type == INT) {
        // Realiza la operación de módulo si el divisor no es cero
        if (op2.integer != 0) {
            result.integer = op1.integer % op2.integer;
        } else {
            fprintf(stderr, "Error: División por cero en la operación de módulo\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Error: La operación de módulo solo se puede realizar con enteros\n");
        exit(EXIT_FAILURE);
    }

    return result;
}


//POTENCIA
estructura potencia(estructura op1, estructura op2) {
    estructura result;
    result.type = FLOAT;
    result.real = pow(getFloatValue(op1), getFloatValue(op2));
    return result;
}



// Concatenación 
char* intToString(int value) {
    int length = snprintf(NULL, 0, "%d", value);
    char* str = malloc(length + 1);
    snprintf(str, length + 1, "%d", value);
    return str;
}

char* floatToString(float value) {
    int length = snprintf(NULL, 0, "%f", value);
    char* str = malloc(length + 1);
    snprintf(str, length + 1, "%f", value);
    return str;
}

char* boolToString(bool value) {
    return value ? strdup("true") : strdup("false");
}

// Operación de concatenación
estructura concatenar(estructura op1, estructura op2) {
    estructura result;
    result.type = STRING;
    
    char *str1 = (op1.type == STRING) ? strdup(op1.string) 
                : (op1.type == INT) ? intToString(op1.integer) 
                : (op1.type == FLOAT) ? floatToString(op1.real)
                : (op1.type == BOOLEAN) ? boolToString(op1.boolean)
                : strdup("");

    char *str2 = (op2.type == STRING) ? strdup(op2.string) 
                : (op2.type == INT) ? intToString(op2.integer) 
                : (op2.type == FLOAT) ? floatToString(op2.real)
                : (op2.type == BOOLEAN) ? boolToString(op2.boolean)
                : strdup("");

    result.string = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result.string, str1);
    strcat(result.string, str2);

    free(str1);
    free(str2);

    return result;
}



// Función calculate
estructura calculate(estructura op1, estructura operator, estructura op2) {
    char op = operator.string[0];
    estructura result;
    
    if (op == '+' && (op1.type == STRING || op2.type == STRING)) {
        return concatenar(op1, op2);
    }

    switch (op) {
        case '+': return sumar(op1, op2);
        case '-': return restar(op1, op2);
        case '*': return multiplicar(op1, op2);
        case '/': return dividir(op1, op2);
        case '^': return potencia(op1, op2);
        case '%': return modulo(op1,op2);
        default:
            fprintf(stderr, "Operación no permitida\n");
            exit(EXIT_FAILURE);
    }
}



//Trigonometria 
float toRadians(float grados) {
    return grados * (PI / 180);
}

estructura trigonometric_op(estructura value, estructura op){
    estructura r;
    r.type = FLOAT;
    float number = (value.type == INT) ? (float)value.integer : value.real;
    number = toRadians(number); 

    if (!strcmp(op.string, "cos")) {
        r.real = cos(number);
    } else if (!strcmp(op.string, "sin")) {
        r.real = sin(number);
    } else if (!strcmp(op.string, "tan")) {
        r.real = tan(number);
    }

    return r;
}


estructura check_boolean(estructura op1, estructura operador, estructura op2) {

    char *op = operador.string;
    estructura r;
    r.type = BOOLEAN;
    float r1, r2;
    
    if (op1.type == INT) 	r1 = op1.integer;
    if (op1.type == FLOAT) 	r1 = op1.real;
    if (op2.type == INT) 	r2 = op2.integer;
    if (op2.type == FLOAT) 	r2 = op2.real;

    if (strlen(op) == 1) {
        if(op[0] == '>') r.boolean = r1 > r2;
        if(op[0] == '<') r.boolean = r1 < r2;
    } 
    else {
        if(op[0] == '>' && op[1] == '=') r.boolean = r1 >= r2;
        if(op[0] == '<' && op[1] == '=') r.boolean = r1 <= r2;
        if(op[0] == '=' && op[1] == '=') r.boolean = r1 == r2;
        if(op[0] == '<' && op[1] == '>') r.boolean = r1 != r2;
    }
    return r;
}


estructura negate(estructura value) {
    if(value.type == INT) 	value.integer = -value.integer;
    if(value.type == FLOAT) 	value.real = -value.real;
    return value;
}


void cambiar_modo_formato(const char* modo) {
    modo_formato = strdup(modo);
}


void initializeArray(const char* arrayName, int size) {
    estructura newArray;
    newArray.type = ARRAY;
    newArray.arraySize = size;
    newArray.array = malloc(sizeof(estructura) * size);
    for (int i = 0; i < size; i++) {
        ((estructura *)newArray.array)[i].type = INT;
        ((estructura *)newArray.array)[i].integer = 0;
    }
    sym_enter(arrayName, &newArray);
}

void assignArrayElement(const char* arrayName, int index, estructura value) {
    estructura arrayVar;
    if (sym_lookup(arrayName, &arrayVar) != SYMTAB_NOT_FOUND && arrayVar.type == ARRAY) {
        if (index >= 0 && index < arrayVar.arraySize) {
            estructura *arrayElement = &(((estructura *)arrayVar.array)[index]);
            *arrayElement = value;
        } else {
            yyerror("Índice de array fuera de rango");
        }
    } else {
        yyerror("Identificador no es un array o no encontrado");
    }
}

int accessArrayElement(const char* arrayName, int index) {
    estructura arrayVar;
    if (sym_lookup(arrayName, &arrayVar) != SYMTAB_NOT_FOUND && arrayVar.type == ARRAY) {
        if (index >= 0 && index < arrayVar.arraySize) {
            estructura *element = &(((estructura *)arrayVar.array)[index]);
            return element->integer;
        } else {
            yyerror("Índice de array fuera de rango");
        }
    } else {
        yyerror("Identificador no es un array o no encontrado");
    }
    return 0; // Valor predeterminado si ocurre un error
}


void printArray(const char* arrayName) {
    estructura arrayVar;
    if (sym_lookup(arrayName, &arrayVar) != SYMTAB_NOT_FOUND && arrayVar.type == ARRAY) {
        estructura newArray;
        newArray.type = ARRAY;
        newArray.arraySize = arrayVar.arraySize;
        newArray.array = malloc(sizeof(estructura) * newArray.arraySize);
        for (int i = 0; i < newArray.arraySize; i++) {
            ((estructura *)newArray.array)[i] = ((estructura *)arrayVar.array)[i];
        }
        print_result(newArray); 
    } else {
        yyerror("Identifier is not an array or not found");
    }
}

void print_result(estructura s) {
    if (s.type == INT) {
        if (strcmp(modo_formato, "hexagesimal") == 0) {
            printf("Type: INT - Value: 0x%X\n", (int)s.integer);
        } else if (strcmp(modo_formato, "octal") == 0) {
            printf("Type: INT - Value: 0o%o\n", (int)s.integer);
        } else {
            printf("Type: INT - Value: %i\n", (int)s.integer);
        }
     }
    if(s.type == FLOAT) 	printf("Type: FLOAT - Value: %f\n", s.real);
    if(s.type == STRING) 	printf("Type: STRING - Value: %s\n", s.string);       
    if(s.type == BOOLEAN){
            if (s.boolean) 	printf("Type: BOOLEAN - Value: true \n"); 
            else 		printf("Type: BOOLEAN - Value: false\n"); 
    }
    if (s.type == ARRAY) {
        printf("Type: ARRAY - Size: %d:\n", s.arraySize);
        for (int i = 0; i < s.arraySize; i++) {
            estructura element = ((estructura*)s.array)[i];
            printf("\tElemento %d: ", i);
            print_result(element);
      	}
    }
}

