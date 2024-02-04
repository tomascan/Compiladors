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
"["			return LBRACKET; 
"]"			return RBRACKET;	
"+"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return ADD; }
"-"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return SUB; }
"*"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return MUL; }
"/"                     { yylval.var.tipo = STRING; yylval.var.string = strdup(yytext); return DIV; }
"mod" 			{ yylval.var.tipo = STRING; yylval.var.string = strdup("%"); return MOD; }
"**"                    { yylval.var.tipo = STRING; yylval.var.string = strdup("^"); return POW; }
"do"			return DO;
"done"			return DONE;
"repeat"		return REPEAT;
"int"     		return TO_INT;
"float"   		return TO_FLOAT;
{commentary}   		{ }         
"/*"                    { BEGIN(COMMENTARY); }
<COMMENTARY>"*/\n" { BEGIN(INITIAL); }
<COMMENTARY>"\n"
<COMMENTARY>.
{int}                   { yylval.var.tipo = INT; yylval.var.integer = atoi(yytext); return VAR; }
{float}                 { yylval.var.tipo = FLOAT; yylval.var.real = atof(yytext); return VAR; }
{identifier} { 	
    yylval.var.string = strdup(yytext);
    estructura s;
    if(sym_lookup(yylval.var.string, &s) != SYMTAB_NOT_FOUND) {
        switch (s.tipo) {
            case ARRAY:
                return ARRAY_ID;
            default:
                return ARITHMETIC_ID;
        }
    } else {
        return ARITHMETIC_ID;
    }
}


.                       { 	printf("[LEXIC ERROR!] Input %s not accepted.\n", yytext); 
				yyterminate(); 
			}
%%
