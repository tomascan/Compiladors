/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 15 "syntax.y"

    #include "functions.h"

#line 53 "syntax.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ASIGN = 258,                   /* ASIGN  */
    POPEN = 259,                   /* POPEN  */
    PCLOSE = 260,                  /* PCLOSE  */
    TTRUE = 261,                   /* TTRUE  */
    FFALSE = 262,                  /* FFALSE  */
    AND = 263,                     /* AND  */
    OR = 264,                      /* OR  */
    NOT = 265,                     /* NOT  */
    DO = 266,                      /* DO  */
    DONE = 267,                    /* DONE  */
    IF = 268,                      /* IF  */
    THEN = 269,                    /* THEN  */
    FI = 270,                      /* FI  */
    ELSE = 271,                    /* ELSE  */
    WHILE = 272,                   /* WHILE  */
    UNTIL = 273,                   /* UNTIL  */
    TO = 274,                      /* TO  */
    IN = 275,                      /* IN  */
    FOR = 276,                     /* FOR  */
    ADD = 277,                     /* ADD  */
    SUB = 278,                     /* SUB  */
    MUL = 279,                     /* MUL  */
    DIV = 280,                     /* DIV  */
    POW = 281,                     /* POW  */
    MOD = 282,                     /* MOD  */
    VAR = 283,                     /* VAR  */
    ARITHMETIC_ID = 284,           /* ARITHMETIC_ID  */
    INTRO = 285,                   /* INTRO  */
    REPEAT = 286,                  /* REPEAT  */
    GT = 287,                      /* GT  */
    LT = 288,                      /* LT  */
    GE = 289,                      /* GE  */
    LE = 290,                      /* LE  */
    EQ = 291,                      /* EQ  */
    NE = 292                       /* NE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "syntax.y"

    estructura var;

#line 111 "syntax.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */
