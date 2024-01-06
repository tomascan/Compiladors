%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "symtab.h"

extern FILE* yyin;
extern int yylineno;
extern void yyerror(char *err);
extern int yylex();
%}

%code requires {
    #include "functions.h"
}

%union {
    estructura var;
};

%token ASIGN POPEN PCLOSE DO DONE 
%token <var> ADD SUB MUL DIV POW MOD VAR ARITHMETIC_ID INTRO REPEAT
%type <var> arithmetic_op1 arithmetic_op2 arithmetic_exp basic_exp mul pow instructions instruction calculator setup

%start calculator

%%

calculator: instructions {generate(1, "HALT");}
;
instructions: instructions instruction 
	| instruction
;

instruction: 
    ARITHMETIC_ID ASIGN arithmetic_exp INTRO { sym_enter($1.string, &$3); generate(3, $1.string, " := ", $3.value); }
    | arithmetic_exp INTRO { put($1); }
    | setup DO INTRO instructions DONE INTRO{
							char* aux2 = malloc(sizeof(int));
    							sprintf(aux2, "%d", $1.integer);
    							generate(4, $1.value, " := ", $1.value, " ADDI 1");
     							generate(6, "IF ", $1.value, " LTI ", $1.string, " GOTO ", aux2); }
     							
;

setup: REPEAT arithmetic_exp	{ $$.value = temporal(); 
			       	generate(2, $$.value, " := 0");
			  	$$.integer = next_quad; 
			  	$$.string = $2.value;
				}

arithmetic_exp: 
    arithmetic_exp arithmetic_op1 mul { if($1.tipo != $3.tipo) 
					{ if($1.tipo == INT) 
						{ 
						  char* temp = $1.value;
						  $1.value = temporal();
						  $1.tipo = FLOAT;
						  generate(4, $1.value, " := ", " I2F ",temp);  
						}
					  else  
						{
						  char* temp = $3.value;
						  $3.tipo = FLOAT;
                                                  $3.value = temporal();
                                                  generate(4, $3.value, " := ", " I2F ",temp);
						}
				        }
				$$=calculate($1,$2,$3);
				$$.value = temporal(); 
		       		generate(5, $$.value, " := ", $1.value, $2.string, $3.value);}
				
	| SUB VAR {
			$2 = set_valor($2);
			$$=negate($2);
			char* numero = $$.value;
			$$.value = temporal(); 
    			generate(4, $$.value, " :=", $$.string, numero);
		  }
	| SUB ARITHMETIC_ID {             estructura r; if(sym_lookup($2.string,&r)==SYMTAB_NOT_FOUND) yyerror("Error sintactico: El identificador no existe");
				    else 
					{ 
					  $$ = negate(r);
    					  $$.value = temporal(); 
    					  generate(4, $$.value, " :=", $$.string, $2.string);
					} 
			}		       
	| mul
; 

mul: mul arithmetic_op2 pow { 
			if($1.tipo != $3.tipo) 
					{ if($1.tipo == INT) 
						{ 
						  char* temp = $1.value;
						  $1.tipo = FLOAT;
						  $1.value = temporal();
						  generate(4, $1.value, " := ", " I2F ",temp);  
						}
					  else  
						{
						  char* temp = $3.value;
						  $3.tipo = FLOAT;
                                                  $3.value = temporal();
                                                  generate(4, $3.value, " := ", " I2F ",temp);
						}
				        }
		       $$=calculate($1,$2,$3);
		       $$.value = temporal(); 
		       generate(5, $$.value, " := ", $1.value, $2.string, $3.value);}
    | pow 
; 
pow: pow POW basic_exp {
				char* temp = temporal();
				generate(2, "i ", " := 1");
				$$ = calculate($1,$2,$3);
				char *exp = malloc(sizeof(int));
				sprintf(exp, "%d" , $3.integer);
				int linea = next_quad;
				generate(6, "IF "," i ", " GEI ", exp , " GOTO " , " FIN ");
				generate(5, temp, " := ", temp, " MULI " ,$1.value);
				generate(4,"i ", " := "," i ", " ADDI 1 ");
				char *line = malloc(sizeof(int));
				sprintf(line, "%d" , linea);
				generate(2, "GOTO ", line);     
			     }
	| basic_exp
;	
basic_exp: ARITHMETIC_ID {if(sym_lookup($1.string,&$$)==SYMTAB_NOT_FOUND) yyerror("Error sintactico: El identificador no existe");
			  else  $$.value = $1.string;}
	| POPEN arithmetic_exp PCLOSE {$$ = $2;} 
	| VAR { $$ = set_valor($1);  }
;
arithmetic_op1: ADD | SUB;
arithmetic_op2: MUL | DIV | MOD;

%%

void yyerror(char *err)
{
    fprintf(stderr, "Error: (%s), line: %i", err, yylineno);
    exit(1);
}

int main(int argc, char **argv)
{
    quads = (char **) malloc(50);
    next_quad = 1;
    next_temp = 1;
    if (argc > 1)
        yyin = fopen(argv[1],"r");
    else
        yyin = stdin;
    yyparse();
    for (int i = 0; i < next_quad-1; i++)
        printf("%s\n", quads[i]);
    return 0;
}

