%option noyywrap
%{
    #include "syntax.tab.h"
    #include "symtab.h"
%}

%x COMMENTARY

sep                 [ \t\r]*
int                 0|[1-9][0-9]*
float               [0-9]*\.[0-9]+([eE][-+]?[0-9]+)?
commentary          "//".*"\n"|"#".*"\n"
identifier          [[a-zA-Z]([a-zA-Z]|[0-9])*

%%

{sep}    
"\n"                    return INTRO; 
":="                    return ASIGN; 
"("                    	return POPEN;
")"                     return PCLOSE;
"+"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return ADD; }
"-"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return SUB; }
"*"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return MUL; }
"/"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return DIV; }
"mod" 			{ yylval.var.tipo = STRING; yylval.var.string = strdup("%"); return MOD; }
"**"                    { yylval.var.tipo = STRING; yylval.var.string = strdup("^"); return POW; }
">"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return GT; }
"<"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return LT; }
">="                    { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return GE; }
"<="                    { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return LE; }
"="                    { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return EQ; }
"<>"                    { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return NE; }
"true"			return TTRUE;
"false"			return FFALSE;
"not"			return NOT;
"and"			return AND;
"or"			return OR;
"do"			return DO;
"done"			return DONE;
"repeat"		return REPEAT;
"for"			return FOR;
"while"			return WHILE;
"until"			return UNTIL;
".."			return TO;
"in"			return IN;
"if"                    return IF;
"then"                  return THEN;
"else"			return ELSE;
"fi"                    return FI;
               
{commentary}   		{ }         
"/*"                    { BEGIN(COMMENTARY); }
<COMMENTARY>"*/\n" { BEGIN(INITIAL); }
<COMMENTARY>"\n"
<COMMENTARY>.
{int}                   { yylval.var.tipo = INT; yylval.var.integer = atoi(yytext); return VAR; }
{float}                 { yylval.var.tipo = FLOAT; yylval.var.real = atof(yytext); return VAR; }

{identifier}            {   yylval.var.tipo = STRING; 
                            yylval.var.string = malloc(sizeof(char) * (yyleng + 1));
                            estructura r;
                            if (yylval.var.string == NULL) {
                                printf("Error de memoria\n");
                                exit(EXIT_FAILURE);
                            }
                            strncpy(yylval.var.string, yytext, yyleng);
                            yylval.var.string[yyleng] = '\0'; // Agrega el carácter nulo
                            return ARITHMETIC_ID; // Devuelve el tipo de token
                        }

.                       { 	printf("[LEXIC ERROR!] Input %s not accepted.\n", yytext); 
				yyterminate(); 
			}
%%
