#ifndef TYPES_H
#define TYPES_H


typedef enum
{
	INT,
	FLOAT,
	STRING,
	ARRAY
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
    int arraySize;
    void *array;
} estructura;

extern char **quads;
extern int next_quad;
extern int next_temp;

#endif /* TYPES_H */

