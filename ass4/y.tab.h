/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    INTEGER = 258,
    CHARACTER = 259,
    STRING = 260,
    FOR = 261,
    WHILE = 262,
    IF = 263,
    GETI = 264,
    GETC = 265,
    GETS = 266,
    PUTI = 267,
    PUTC = 268,
    PUTS = 269,
    PUTI_ = 270,
    PUTC_ = 271,
    PUTS_ = 272,
    GRID = 273,
    GRID_GET = 274,
    GRID_ROTATE = 275,
    IFX = 276,
    ELSE = 277,
    VARIABLE = 278,
    AND = 279,
    OR = 280,
    GE = 281,
    LE = 282,
    EQ = 283,
    NE = 284,
    UMINUS = 285
  };
#endif
/* Tokens.  */
#define INTEGER 258
#define CHARACTER 259
#define STRING 260
#define FOR 261
#define WHILE 262
#define IF 263
#define GETI 264
#define GETC 265
#define GETS 266
#define PUTI 267
#define PUTC 268
#define PUTS 269
#define PUTI_ 270
#define PUTC_ 271
#define PUTS_ 272
#define GRID 273
#define GRID_GET 274
#define GRID_ROTATE 275
#define IFX 276
#define ELSE 277
#define VARIABLE 278
#define AND 279
#define OR 280
#define GE 281
#define LE 282
#define EQ 283
#define NE 284
#define UMINUS 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "bc.y"

    int iValue;                 /* integer value */
    char* sPtr;                 /* string pointer */
    nodeType *nPtr;             /* node pointer */

#line 123 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
