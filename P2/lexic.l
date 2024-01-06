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
"mod"                   { yylval.var.tipo = STRING; yylval.var.string = "%"; return MOD; }
"**"                    { yylval.var.tipo = STRING; yylval.var.string = "^"; return POW; }
"do"			return DO;
"done"			return DONE;
"repeat"		return REPEAT;
               
{commentary}   		{ }         
"/*"                    { BEGIN(COMMENTARY); }
<COMMENTARY>"*/\n" { BEGIN(INITIAL); }
<COMMENTARY>"\n"
<COMMENTARY>.
{int}                   { yylval.var.tipo = INT; yylval.var.integer = atoi(yytext); return VAR; }
{float}                 { yylval.var.tipo = FLOAT; yylval.var.real = atof(yytext); return VAR; }

{identifier}            {   yylval.var.tipo = STRING; 
                            yylval.var.string = malloc(sizeof(char) * (yyleng + 1));
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
