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

%token ASIGN POPEN PCLOSE TTRUE FFALSE AND OR NOT DO DONE IF THEN FI ELSE WHILE UNTIL TO IN FOR 
%token <var> ADD SUB MUL DIV POW MOD VAR ARITHMETIC_ID INTRO REPEAT GT LT GE LE EQ NE
%type <var> op1 op2 op3 arithmetic_exp boolean_exp basic_exp boolean_logic mul pow instructions instruction calculator setup expresion else and not x y z

%start calculator

%%

calculator: instructions {
				complete($1.next_list, next_quad);
				generate(1,"HALT");
			   }
;
instructions: instructions x instruction {complete($1.next_list, $2.repeat);
					        $$.next_list = $3.next_list; }
	| instruction 
;
instruction:  ARITHMETIC_ID ASIGN arithmetic_exp INTRO {
						sym_enter($1.string, &$3);  
						generate(3, $1.string, " := " ,$3.value);
					    
}
	| expresion INTRO { put($1); }
	| setup DO INTRO instructions DONE INTRO{
							char* aux2 = malloc(sizeof(int));
    							sprintf(aux2, "%d", $1.integer);
    							generate(4, $1.value, " := ", $1.value, " ADDI 1");
     							generate(6, "IF ", $1.value, " LTI ", $1.string, " GOTO ", aux2);  }
 	| WHILE x boolean_exp DO
    	INTRO x instructions DONE INTRO   {
						
                                                complete($7.next_list, $2.repeat);
                                                complete($3.true_list, $6.repeat);
                                                $$.next_list = $3.false_list;
                                                char* aux = malloc(5);
                                                sprintf(aux, "%i", $2.repeat);
                                                generate(2, "GOTO ", aux);
          	                              }
	| DO INTRO x instructions UNTIL POPEN  
    boolean_exp PCLOSE INTRO			{
							complete($7.true_list, $3.repeat);
							$$.next_list = combine($7.false_list, $4.next_list);
						}
	| IF POPEN boolean_exp PCLOSE 
		THEN INTRO x instructions else 
		FI INTRO			{ 	complete($3.true_list, $7.repeat);
    								if ($9.repeat == -1)
									$$.next_list = combine($3.false_list, $8.next_list);
								else {
									$$.next_list = combine($8.next_list, $9.next_list); 
    									complete($3.false_list, $9.repeat); 
								}
    						}
	| z DO INTRO instructions DONE INTRO           {	complete($4.next_list, next_quad); 
								char *aux = malloc(6);
								generate(4, $1.string, " := ", $1.string, " ADDI 1");  
								sprintf(aux, "%i", $1.integer); 
								generate(2, "GOTO ", aux); 
								$$.next_list = $1.next_list; 
    							}   		

;
z: FOR POPEN ARITHMETIC_ID IN arithmetic_exp 
TO arithmetic_exp PCLOSE                   { 	

								estructura r; 
								generate(3, $3.string, " := ", $5.value);
								$$.value = $5.value; 
								$$.integer = next_quad; 
								char *aux = malloc(6);
								sprintf(aux, "%i", $$.integer+2); 
								generate(6, "IF ", $3.string, " LEI ", $7.value, " GOTO ", aux); 
								$$.next_list = crearLista(next_quad); 
								generate(1, "GOTO"); 
								$$.string = $3.string;
					    	    }
;
else: 							{ $$.repeat = -1; }
    | ELSE INTRO y x instructions 			{ 	$$.next_list = combine($3.next_list, $5.next_list); 
    								$$.repeat = $4.repeat; 
    							}		
;
expresion: arithmetic_exp
	| boolean_exp
;
setup: REPEAT arithmetic_exp { $$.value = temporal(); 
			  generate(2, $$.value, " := 0");
			  $$.integer = next_quad; 
			  $$.string = $2.value;
			}
;
arithmetic_exp: arithmetic_exp op1 mul { if($1.tipo != $3.tipo) 
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
				calculate($1,$2,$3);
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

mul: mul op2 pow { 
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
		       calculate($1,$2,$3);
		       $$.value = temporal(); 
		       generate(5, $$.value, " := ", $1.value, $2.string, $3.value);}
    | pow
; 
pow: pow POW basic_exp {
				char* temp = temporal();
				char* cont = temporal();
				generate(2, temp, " := 1");
				generate(2, cont, " := 1");
				calculate($1,$2,$3);
				char *exp = malloc(sizeof(int));
				sprintf(exp, "%d" , $3.integer);
				int linea = next_quad;
				char *fin = malloc(sizeof(int));;
				sprintf(fin, "%d", linea+4);
				generate(6, "IF ",cont, " GEI ", exp , " GOTO " , fin);
				generate(5, temp, " := ", temp, " MULI " ,$1.value);
				generate(4,"i ", " := ",cont, " ADDI 1 ");
				char *line = malloc(sizeof(int));
				sprintf(line, "%d" , linea);
				generate(2, "GOTO ", line);  
				$$.value = temp;   
			     }
	| basic_exp
;	
basic_exp: ARITHMETIC_ID {if(sym_lookup($1.string,&$$)==SYMTAB_NOT_FOUND) yyerror("Error sintactico: El identificador no existe");
			  else  $$.value = $1.string;}
	| POPEN arithmetic_exp PCLOSE {$$ = $2;} 
	| VAR {$$ = set_valor($1);  }
;
boolean_exp: boolean_exp OR x and { $$.boolean = ($1.boolean | $4.boolean); 
			      $$.tipo=BOOLEAN;
			      $$.true_list = combine($1.true_list, $4.true_list); 
			      $$.false_list = $4.false_list; 
			      complete($1.false_list, $3.repeat); 
			    }
	| and
;
and: and AND x not { 
		     $$.boolean = ($1.boolean & $4.boolean); 
		     $$.tipo = BOOLEAN;
		     $$.true_list = $4.true_list;
		     $1.false_list = combine($1.false_list, $4.false_list);
		     }
	| not
;
not: NOT boolean_logic { $$.boolean = !($2.boolean); 
			   $$.tipo=BOOLEAN;
			   $$.true_list = $2.false_list; 
			   $$.false_list = $2.true_list;
			 }
	| boolean_logic
;
boolean_logic: arithmetic_exp op3 arithmetic_exp { 
						
					    $$ = boolean_calculate($1,$2,$3);
					    $$.true_list = crearLista(next_quad);
					    $$.false_list = crearLista(next_quad+1);
					    generate(5, "IF ", $1.value, $2.string, $3.value, " GOTO");
					    generate(1, "GOTO");
						
					    } 
	| TTRUE		      	      {$$.boolean=true; 
				       $$.tipo=BOOLEAN;
				       $$.value = "TRUE";
    				       $$.true_list = crearLista(next_quad);
    				       $$.false_list = NULL;
    				       generate(1, "GOTO");
				       }
	| FFALSE		      {$$.boolean=false; 
				       $$.tipo=BOOLEAN;
				       $$.value = "FALSE";
    				       $$.true_list = NULL;
    				       $$.false_list = crearLista(next_quad);
    				       generate(1, "GOTO");
					}
	| POPEN boolean_exp PCLOSE     { $$=$2;}
; 
x: { $$.repeat = next_quad; }
;
y: { $$.next_list = crearLista(next_quad);
     generate(1, "GOTO"); }
;

op1: ADD | SUB;
op2: MUL | DIV | MOD;
op3: GT | LT | GE | LE | EQ | NE;

%%

void yyerror(char *err)
{
    fprintf(stderr, "Error: (%s), line: %i", err, yylineno);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening file");
            return 1;
        }
    } else {
        yyin = stdin;
    }

    quads = (char **) malloc(50 * sizeof(char*));
    if (!quads) {
        perror("Memory allocation failed");
        fclose(yyin);
        return 1;
    }

    next_quad = 1;
    next_temp = 1;

    yyparse();

    for (int i = 0; i < next_quad-1; i++) {
        printf("%s\n", quads[i]);
        free(quads[i]);
    }

    free(quads);
    if (yyin != stdin) {
        fclose(yyin);
    }

    return 0;
}

