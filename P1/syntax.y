%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "symtab.h"

extern FILE* yyin;
extern int yylineno;
void yyerror(const char *err);
extern int yylex();
%}

%code requires {
    #include "functions.h"
}

%union {
    estructura var;
}

%token ASIGN OPEN CLOSE TTRUE TFALSE AND OR NOT DEC_MODE OCT_MODE HEX_MODE STRLEN
%token <var> ADD SUB MUL DIV POW MOD GT LT GE LE EQ NE VAR BOOLEAN_ID ARITHMETIC_ID INTRO COS SIN TAN
%type <var> expression arithmetic_op1 arithmetic_op2 boolean_op arithmetic_exp boolean_exp strlen_exp trigonometric_op sum mul pow and not top_bool instructions instruction

%start instructions

%%

instructions: instructions instruction
    | instruction
;

instruction: 
    ARITHMETIC_ID ASIGN expression INTRO { sym_enter($1.string, &$3); printf("Variable: \"%s\" - ", $1.string); print_result($3); }
    | BOOLEAN_ID ASIGN expression INTRO   { sym_enter($1.string, &$3); printf("Var \"%s\" - ", $1.string); print_result($3); }
    | expression INTRO                   { print_result($1); }
    | DEC_MODE INTRO                { cambiar_modo_formato("decimal"); }
    | OCT_MODE INTRO                  { cambiar_modo_formato("octal"); }
    | HEX_MODE INTRO                    { cambiar_modo_formato("hexagesimal"); }
;

expression: 
    arithmetic_exp 
    | boolean_exp 
    | strlen_exp
;

arithmetic_exp: 
    arithmetic_exp arithmetic_op1 sum { $$ = calculate($1, $2, $3); } 
    | sum 
;

sum: 
    sum arithmetic_op2 mul { $$ = calculate($1, $2, $3); } 
    | SUB VAR             { $$ = negate($2); }
    | SUB ARITHMETIC_ID   { estructura s; if(sym_lookup($2.string, &s) == SYMTAB_NOT_FOUND) yyerror("Identifier does not exist"); $$ = negate(s); }
    | mul 
;

mul: 
    mul POW pow { $$ = calculate($1, $2, $3); } 
    | pow 
;

pow: 
    ARITHMETIC_ID { if(sym_lookup($1.string, &$$) == SYMTAB_NOT_FOUND) yyerror("Identifier does not exist"); }
    | VAR         { $$ = $1; }
    | OPEN arithmetic_exp CLOSE { $$ = $2; }
    | trigonometric_op OPEN arithmetic_exp CLOSE { $$ = trigonometric_op($3, $1); }
;

boolean_exp: boolean_exp OR and               		{ $$.boolean = $1.boolean; $$.type = BOOLEAN; }
    | and                                   
;

and: and AND not                          		{ $$.boolean = $1.boolean; $$.type = BOOLEAN; }
    | not                                   
;

not: NOT top_bool                                	{ $$ = $2; $$.boolean = !$$.boolean; $$.type = BOOLEAN; }
    | top_bool                                   
;

top_bool: arithmetic_exp boolean_op arithmetic_exp  	{ $$ = check_boolean($1, $2, $3); } 
    | TTRUE                                  		{ $$.boolean = true; $$.type = BOOLEAN; }
    | TFALSE                                  		{ $$.boolean = false; $$.type = BOOLEAN; }
    | OPEN boolean_exp CLOSE                 		{ $$ = $2; }
    | BOOLEAN_ID                            		{ if(sym_lookup($1.string, &$$) == SYMTAB_NOT_FOUND) yyerror("Identifier does not exist"); }
;

strlen_exp:
    STRLEN OPEN ARITHMETIC_ID CLOSE {
       					 estructura s;
        				if(sym_lookup($3.string, &s) == SYMTAB_NOT_FOUND)
          					  yyerror("Identifier does not exist");
        				if (s.type != STRING)
            					yyerror("strlen requires a string");
        				$$.integer = strlen(s.string);
       					$$.type = INT;
   				    }
    | STRLEN OPEN VAR CLOSE {
        $$.integer = strlen($3.string);
        $$.type = INT;
    }			    
;


arithmetic_op1: ADD | SUB;                                

arithmetic_op2: MUL | DIV | MOD;      

trigonometric_op: SIN | COS | TAN;                           

boolean_op: GT | LT | GE | LE | EQ | NE;

%%


void yyerror(const char *err)
{
    fprintf(stderr, "Error: (%s), line: %i", err, yylineno);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc > 1)
        yyin = fopen(argv[1],"r");
    else
        yyin = stdin;
    yyparse();
    return 0;
}
