#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum
{
	INT,
	FLOAT,
	STRING,
	BOOLEAN,
	ARRAY
}type;

typedef struct{
    char *string;
    int integer;
    float real;
    bool boolean;
    type type;
    int arraySize;
    void *array;
} estructura;

#endif /* TYPES_H */

