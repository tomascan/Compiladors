#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum
{
	INT,
	FLOAT,
	STRING,
	BOOLEAN
}type;

typedef struct{
    char *string;
    int integer;
    float real;
    bool boolean;
    type type;
} estructura;

#endif /* TYPES_H */

