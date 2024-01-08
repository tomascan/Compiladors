#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

typedef enum
{
	INT,
	FLOAT,
	STRING,
	BOOLEAN	
}type;


typedef struct quad_struct{
    int num_quad;
    struct quad_struct* next_quad;
} quad;



typedef struct{
    char *string;
    int integer;
    float real;
    char *value;
    type tipo;
    bool boolean;
    int repeat;
    quad* next_list;
    quad* true_list;
    quad* false_list;
} estructura;

extern char **quads;
extern int next_quad;
extern int next_temp;

#endif /* TYPES_H */

