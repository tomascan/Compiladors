%option noyywrap
%{
    #include "syntax.tab.h"
    #include "symtab.h"
    #include <string.h> 
    char* modo_formato = "decimal";
%}

%{
#define PI 3.14159265358979323846
#define E 2.7818281828459045
%}


%x COMMENTARY

sep                 [ \t\r]*
int                 0|[1-9][0-9]*
float               [0-9]*\.[0-9]+([eE][-+]?[0-9]+)?
string              \".*\"
commentary          "//".*"\n"|"#".*"\n"
identifier          [[a-zA-Z]([a-zA-Z]|[0-9])*

%%

"\n"                    return INTRO; 
":="                    return ASIGN; 
"("                     return OPEN;
")"                     return CLOSE;
"["			return LBRACKET;
"]"			return RBRACKET;
"+"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return ADD; }
"-"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return SUB; }
"*"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return MUL; }
"/"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return DIV; }
"mod"                   { yylval.var.type = STRING; yylval.var.string = "%"; return MOD; }
"**"                    { yylval.var.type = STRING; yylval.var.string = "^"; return POW; }
"sin"			{ yylval.var.type = STRING; yylval.var.string = "sin"; return SIN; }
"cos"			{ yylval.var.type = STRING; yylval.var.string = "cos"; return COS; }
"tan"			{ yylval.var.type = STRING; yylval.var.string = "tan"; return TAN; }
">"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return GT; }
"<"                     { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return LT; }
">="                    { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return GE; }
"<="                    { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return LE; }
"=="                    { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return EQ; }
"<>"                    { yylval.var.type = STRING; yylval.var.string = strdup(yytext); return NE; }
"true"                  return TTRUE;
"false"                 return TFALSE;
"not"                   return NOT;
"and"                   return AND;
"or"                    return OR;
"strlen" 		return  STRLEN;
"decimal"               { modo_formato = "decimal"; return DEC_MODE; } 
"octal"                 { modo_formato = "octal"; return OCT_MODE; }   
"hexagesimal"           { modo_formato = "hexagesimal"; return HEX_MODE; } 
{sep}                   
{commentary}            
"/*"                    { BEGIN(COMMENTARY); }
<COMMENTARY>"*/\n" { BEGIN(INITIAL); }
<COMMENTARY>"\n"
<COMMENTARY>.

{int}                   { yylval.var.type = INT; yylval.var.integer = atoi(yytext); return VAR; }
"PI" 			{ yylval.var.type = FLOAT; yylval.var.real = PI; return VAR; }
"E" 			{ yylval.var.type = FLOAT; yylval.var.real = E; return VAR; }
{float}                 { yylval.var.type = FLOAT; yylval.var.real = atof(yytext); return VAR; }

{string}                { 	yylval.var.type = STRING; 
				yylval.var.string = (char*)malloc(yyleng-2); 
				strncpy(yylval.var.string, yytext+1, yyleng-2); 
				return VAR; 
			}
{identifier} { 	
    yylval.var.string = strdup(yytext);
    estructura s;
    if(sym_lookup(yylval.var.string, &s) != SYMTAB_NOT_FOUND) {
        switch (s.type) {
            case ARRAY:
                return ARRAY_ID;
            case BOOLEAN:
                return BOOLEAN_ID;
            default:
                return ARITHMETIC_ID;
        }
    } else {
        return ARITHMETIC_ID;
    }
}

.                       { 	printf("[ERROR] Input %s not accepted.\n", yytext); 
				yyterminate(); 
			}
%%

