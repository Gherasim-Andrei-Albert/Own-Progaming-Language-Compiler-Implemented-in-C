/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    FLOAT = 259,
    INT = 260,
    CHAR = 261,
    BOOL = 262,
    PLUS = 263,
    MINUS = 264,
    DIV = 265,
    STAR = 266,
    GT = 267,
    GT_OR_EQUAL = 268,
    LT = 269,
    LT_OR_EQUAL = 270,
    EQUAL = 271,
    AND = 272,
    OR = 273,
    NOT = 274,
    L_ROUND_PARA = 275,
    R_ROUND_PARA = 276,
    L_SQUARE_PARA = 277,
    R_SQUARE_PARA = 278,
    L_COURLY_PARA = 279,
    R_COURLY_PARA = 280,
    FOR = 281,
    IF = 282,
    WHILE = 283,
    ELSE = 284,
    PREDEFINED_TYPE = 285,
    ASSIGN = 286,
    COMMA = 287,
    SEMI_COLON = 288,
    DOT = 289,
    CONST = 290,
    START_MAIN = 291,
    START_VARIABLES = 292,
    START_STRUCTS = 293,
    START_FUNCTIONS = 294
  };
#endif
/* Tokens.  */
#define ID 258
#define FLOAT 259
#define INT 260
#define CHAR 261
#define BOOL 262
#define PLUS 263
#define MINUS 264
#define DIV 265
#define STAR 266
#define GT 267
#define GT_OR_EQUAL 268
#define LT 269
#define LT_OR_EQUAL 270
#define EQUAL 271
#define AND 272
#define OR 273
#define NOT 274
#define L_ROUND_PARA 275
#define R_ROUND_PARA 276
#define L_SQUARE_PARA 277
#define R_SQUARE_PARA 278
#define L_COURLY_PARA 279
#define R_COURLY_PARA 280
#define FOR 281
#define IF 282
#define WHILE 283
#define ELSE 284
#define PREDEFINED_TYPE 285
#define ASSIGN 286
#define COMMA 287
#define SEMI_COLON 288
#define DOT 289
#define CONST 290
#define START_MAIN 291
#define START_VARIABLES 292
#define START_STRUCTS 293
#define START_FUNCTIONS 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 775 "merged.y" /* yacc.c:1909  */

    char* text;
    struct Tree* treeType;

#line 137 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
