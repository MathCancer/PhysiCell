/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with CTBNDL or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum CTBNDLtokentype {
     IDENTIFIER = 258,
     VARIABLE = 259,
     STRING = 260,
     DOUBLE = 261,
     INTEGER = 262,
     LOGAND = 263,
     LOGOR = 264,
     LOGXOR = 265,
     LOGNOT = 266,
     EQUAL = 267,
     NOT_EQUAL = 268,
     NODE = 269,
     GTEQ = 270,
     LTEQ = 271
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define VARIABLE 259
#define STRING 260
#define DOUBLE 261
#define INTEGER 262
#define LOGAND 263
#define LOGOR 264
#define LOGXOR 265
#define LOGNOT 266
#define EQUAL 267
#define NOT_EQUAL 268
#define NODE 269
#define GTEQ 270
#define LTEQ 271




/* Copy the first part of user declarations.  */
#line 2 "BooleanGrammar.y"

/*
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2011-2020 Institut Curie, 26 rue d'Ulm, Paris, France       #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################

   Module:
     BooleanGrammar.y

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr> 
   Date:
     January-March 2011
*/

#define _POSIX_SOURCE 1
#include "BooleanGrammar.h"

extern int CTBNDLlex();
static void CTBNDLerror(const char *s);
static Network* current_network;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 59 "BooleanGrammar.y"
{
  std::vector<NodeDecl*>* node_decl_list;
  NodeDecl* node_decl;
  std::vector<NodeDeclItem*>* node_decl_item_list;
  NodeDeclItem* node_decl_item;
  Expression* expr;
  ArgumentList* arg_list;
  char* str;
  double d;
  long long l;
}
/* Line 193 of yacc.c.  */
#line 197 "BooleanGrammar.tab.c"
	YYSTYPE;
# define CTBNDLstype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 210 "BooleanGrammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 CTBNDLtype_uint8;
#else
typedef unsigned char CTBNDLtype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 CTBNDLtype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char CTBNDLtype_int8;
#else
typedef short int CTBNDLtype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 CTBNDLtype_uint16;
#else
typedef unsigned short int CTBNDLtype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 CTBNDLtype_int16;
#else
typedef short int CTBNDLtype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined CTBNDLoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined CTBNDLoverflow || YYERROR_VERBOSE */


#if (! defined CTBNDLoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union CTBNDLalloc
{
  CTBNDLtype_int16 CTBNDLss;
  YYSTYPE CTBNDLvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union CTBNDLalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (CTBNDLtype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T CTBNDLi;				\
	  for (CTBNDLi = 0; CTBNDLi < (Count); CTBNDLi++)	\
	    (To)[CTBNDLi] = (From)[CTBNDLi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T CTBNDLnewbytes;						\
	YYCOPY (&CTBNDLptr->Stack, Stack, CTBNDLsize);				\
	Stack = &CTBNDLptr->Stack;						\
	CTBNDLnewbytes = CTBNDLstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	CTBNDLptr += CTBNDLnewbytes / sizeof (*CTBNDLptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   99

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  54
/* YYNRULES -- Number of states.  */
#define YYNSTATES  93

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   271

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? CTBNDLtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const CTBNDLtype_uint8 CTBNDLtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,     2,     2,     2,     2,     2,
      24,    25,    29,    26,    20,    27,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,    22,
      31,    21,    32,    34,    23,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    33,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const CTBNDLtype_uint8 CTBNDLprhs[] =
{
       0,     0,     3,     5,     8,    14,    19,    24,    26,    28,
      30,    33,    38,    43,    45,    48,    50,    52,    54,    58,
      60,    65,    69,    71,    75,    77,    80,    83,    86,    89,
      91,    95,    99,   101,   105,   109,   111,   115,   119,   123,
     127,   129,   133,   137,   139,   143,   145,   149,   151,   155,
     159,   161,   167,   169,   170
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const CTBNDLtype_int8 CTBNDLrhs[] =
{
      36,     0,    -1,    37,    -1,    36,    37,    -1,    14,     3,
      17,    39,    18,    -1,    14,     3,    17,    18,    -1,     3,
      38,    53,    54,    -1,    19,    -1,    20,    -1,    40,    -1,
      39,    40,    -1,     3,    21,    53,    22,    -1,     3,    21,
       5,    22,    -1,     3,    -1,    23,     3,    -1,     4,    -1,
       7,    -1,     6,    -1,    24,    53,    25,    -1,    41,    -1,
       3,    24,    43,    25,    -1,     3,    24,    25,    -1,    52,
      -1,    43,    20,    52,    -1,    42,    -1,    26,    44,    -1,
      27,    44,    -1,    28,    44,    -1,    11,    44,    -1,    44,
      -1,    45,    29,    44,    -1,    45,    30,    44,    -1,    45,
      -1,    46,    26,    45,    -1,    46,    27,    45,    -1,    46,
      -1,    47,    31,    46,    -1,    47,    32,    46,    -1,    47,
      16,    46,    -1,    47,    15,    46,    -1,    47,    -1,    48,
      12,    47,    -1,    48,    13,    47,    -1,    48,    -1,    49,
       8,    48,    -1,    49,    -1,    50,     9,    49,    -1,    50,
      -1,    51,    10,    50,    -1,    51,    33,    50,    -1,    51,
      -1,    51,    34,    53,    19,    52,    -1,    52,    -1,    -1,
      22,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const CTBNDLtype_uint16 CTBNDLrline[] =
{
       0,    98,    98,   101,   106,   116,   122,   138,   140,   144,
     149,   156,   161,   168,   174,   179,   184,   188,   192,   198,
     202,   207,   214,   219,   226,   230,   234,   238,   242,   248,
     252,   256,   262,   266,   270,   276,   280,   284,   288,   292,
     298,   302,   306,   312,   316,   322,   326,   332,   336,   340,
     346,   350,   356,   362,   363
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const CTBNDLtname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "VARIABLE", "STRING",
  "DOUBLE", "INTEGER", "LOGAND", "LOGOR", "LOGXOR", "LOGNOT", "EQUAL",
  "NOT_EQUAL", "NODE", "GTEQ", "LTEQ", "'{'", "'}'", "':'", "','", "'='",
  "';'", "'@'", "'('", "')'", "'+'", "'-'", "'!'", "'*'", "'/'", "'<'",
  "'>'", "'^'", "'?'", "$accept", "translation_unit", "node_decl",
  "colon_comma", "node_decl_item_list", "node_decl_item",
  "primary_expression", "postfix_expression", "argument_expression_list",
  "unary_expression", "multiplicative_expression", "additive_expression",
  "relational_expression", "equality_expression", "logical_and_expression",
  "logical_or_expression", "logical_xor_expression",
  "conditional_expression", "expression", "term_opt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const CTBNDLtype_uint16 CTBNDLtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   123,   125,    58,
      44,    61,    59,    64,    40,    41,    43,    45,    33,    42,
      47,    60,    62,    94,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const CTBNDLtype_uint8 CTBNDLr1[] =
{
       0,    35,    36,    36,    37,    37,    37,    38,    38,    39,
      39,    40,    40,    41,    41,    41,    41,    41,    41,    42,
      42,    42,    43,    43,    44,    44,    44,    44,    44,    45,
      45,    45,    46,    46,    46,    47,    47,    47,    47,    47,
      48,    48,    48,    49,    49,    50,    50,    51,    51,    51,
      52,    52,    53,    54,    54
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const CTBNDLtype_uint8 CTBNDLr2[] =
{
       0,     2,     1,     2,     5,     4,     4,     1,     1,     1,
       2,     4,     4,     1,     2,     1,     1,     1,     3,     1,
       4,     3,     1,     3,     1,     2,     2,     2,     2,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     5,     1,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const CTBNDLtype_uint8 CTBNDLdefact[] =
{
       0,     0,     0,     0,     2,     7,     8,     0,     0,     1,
       3,    13,    15,    17,    16,     0,     0,     0,     0,     0,
       0,    19,    24,    29,    32,    35,    40,    43,    45,    47,
      50,    52,    53,     0,     0,    28,    14,     0,    25,    26,
      27,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     6,     0,     5,
       0,     9,    21,     0,    22,    18,    30,    31,    33,    34,
      39,    38,    36,    37,    41,    42,    44,    46,    48,    49,
       0,     0,     4,    10,     0,    20,     0,     0,     0,    23,
      51,    12,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const CTBNDLtype_int8 CTBNDLdefgoto[] =
{
      -1,     3,     4,     7,    60,    61,    21,    22,    63,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    57
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -34
static const CTBNDLtype_int8 CTBNDLpact[] =
{
       1,    36,     0,    58,   -34,   -34,   -34,    43,    -1,   -34,
     -34,    21,   -34,   -34,   -34,    43,    45,    43,    43,    43,
      43,   -34,   -34,   -34,    33,    51,    -7,    67,    49,    56,
      26,   -34,    46,    23,     7,   -34,   -34,    61,   -34,   -34,
     -34,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,   -34,   -34,    62,   -34,
      34,   -34,   -34,    -8,   -34,   -34,   -34,   -34,    33,    33,
      51,    51,    51,    51,    -7,    -7,    67,    49,    56,    56,
      68,    16,   -34,   -34,    43,   -34,    43,    66,    70,   -34,
     -34,   -34,   -34
};

/* YYPGOTO[NTERM-NUM].  */
static const CTBNDLtype_int8 CTBNDLpgoto[] =
{
     -34,   -34,    86,   -34,   -34,    39,   -34,   -34,   -34,   -13,
      38,    28,    35,    42,    44,    37,   -34,   -33,   -17,   -34
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const CTBNDLtype_uint8 CTBNDLtable[] =
{
      37,    64,    35,     8,     1,    38,    39,    40,    45,    46,
      11,    12,    84,    13,    14,     2,    33,    85,    15,    11,
      12,    87,    13,    14,    47,    48,    58,    15,    66,    67,
      16,    17,    62,    18,    19,    20,    53,    58,    80,    16,
      17,    59,    18,    19,    20,    34,    11,    12,    36,    13,
      14,    89,    82,    90,    15,     5,     6,    51,     9,    54,
      55,     1,    41,    42,    88,    52,    16,    17,    56,    18,
      19,    20,     2,    70,    71,    72,    73,    43,    44,    49,
      50,    68,    69,    81,    74,    75,    65,    86,    91,    10,
      78,    79,    92,    76,     0,     0,    77,     0,     0,    83
};

static const CTBNDLtype_int8 CTBNDLcheck[] =
{
      17,    34,    15,     3,     3,    18,    19,    20,    15,    16,
       3,     4,    20,     6,     7,    14,    17,    25,    11,     3,
       4,     5,     6,     7,    31,    32,     3,    11,    41,    42,
      23,    24,    25,    26,    27,    28,    10,     3,    55,    23,
      24,    18,    26,    27,    28,    24,     3,     4,     3,     6,
       7,    84,    18,    86,    11,    19,    20,     8,     0,    33,
      34,     3,    29,    30,    81,     9,    23,    24,    22,    26,
      27,    28,    14,    45,    46,    47,    48,    26,    27,    12,
      13,    43,    44,    21,    49,    50,    25,    19,    22,     3,
      53,    54,    22,    51,    -1,    -1,    52,    -1,    -1,    60
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const CTBNDLtype_uint8 CTBNDLstos[] =
{
       0,     3,    14,    36,    37,    19,    20,    38,     3,     0,
      37,     3,     4,     6,     7,    11,    23,    24,    26,    27,
      28,    41,    42,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    17,    24,    44,     3,    53,    44,    44,
      44,    29,    30,    26,    27,    15,    16,    31,    32,    12,
      13,     8,     9,    10,    33,    34,    22,    54,     3,    18,
      39,    40,    25,    43,    52,    25,    44,    44,    45,    45,
      46,    46,    46,    46,    47,    47,    48,    49,    50,    50,
      53,    21,    18,    40,    20,    25,    19,     5,    53,    52,
      52,    22,    22
};

#define CTBNDLerrok		(CTBNDLerrstatus = 0)
#define CTBNDLclearin	(CTBNDLchar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto CTBNDLacceptlab
#define YYABORT		goto CTBNDLabortlab
#define YYERROR		goto CTBNDLerrorlab


/* Like YYERROR except do call CTBNDLerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto CTBNDLerrlab

#define YYRECOVERING()  (!!CTBNDLerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (CTBNDLchar == YYEMPTY && CTBNDLlen == 1)				\
    {								\
      CTBNDLchar = (Token);						\
      CTBNDLlval = (Value);						\
      CTBNDLtoken = YYTRANSLATE (CTBNDLchar);				\
      YYPOPSTACK (1);						\
      goto CTBNDLbackup;						\
    }								\
  else								\
    {								\
      CTBNDLerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `CTBNDLlex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX CTBNDLlex (YYLEX_PARAM)
#else
# define YYLEX CTBNDLlex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (CTBNDLdebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (CTBNDLdebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      CTBNDL_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
CTBNDL_symbol_value_print (FILE *CTBNDLoutput, int CTBNDLtype, YYSTYPE const * const CTBNDLvaluep)
#else
static void
CTBNDL_symbol_value_print (CTBNDLoutput, CTBNDLtype, CTBNDLvaluep)
    FILE *CTBNDLoutput;
    int CTBNDLtype;
    YYSTYPE const * const CTBNDLvaluep;
#endif
{
  if (!CTBNDLvaluep)
    return;
# ifdef YYPRINT
  if (CTBNDLtype < YYNTOKENS)
    YYPRINT (CTBNDLoutput, CTBNDLtoknum[CTBNDLtype], *CTBNDLvaluep);
# else
  YYUSE (CTBNDLoutput);
# endif
  switch (CTBNDLtype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
CTBNDL_symbol_print (FILE *CTBNDLoutput, int CTBNDLtype, YYSTYPE const * const CTBNDLvaluep)
#else
static void
CTBNDL_symbol_print (CTBNDLoutput, CTBNDLtype, CTBNDLvaluep)
    FILE *CTBNDLoutput;
    int CTBNDLtype;
    YYSTYPE const * const CTBNDLvaluep;
#endif
{
  if (CTBNDLtype < YYNTOKENS)
    YYFPRINTF (CTBNDLoutput, "token %s (", CTBNDLtname[CTBNDLtype]);
  else
    YYFPRINTF (CTBNDLoutput, "nterm %s (", CTBNDLtname[CTBNDLtype]);

  CTBNDL_symbol_value_print (CTBNDLoutput, CTBNDLtype, CTBNDLvaluep);
  YYFPRINTF (CTBNDLoutput, ")");
}

/*------------------------------------------------------------------.
| CTBNDL_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
CTBNDL_stack_print (CTBNDLtype_int16 *bottom, CTBNDLtype_int16 *top)
#else
static void
CTBNDL_stack_print (bottom, top)
    CTBNDLtype_int16 *bottom;
    CTBNDLtype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (CTBNDLdebug)							\
    CTBNDL_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
CTBNDL_reduce_print (YYSTYPE *CTBNDLvsp, int CTBNDLrule)
#else
static void
CTBNDL_reduce_print (CTBNDLvsp, CTBNDLrule)
    YYSTYPE *CTBNDLvsp;
    int CTBNDLrule;
#endif
{
  int CTBNDLnrhs = CTBNDLr2[CTBNDLrule];
  int CTBNDLi;
  unsigned long int CTBNDLlno = CTBNDLrline[CTBNDLrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     CTBNDLrule - 1, CTBNDLlno);
  /* The symbols being reduced.  */
  for (CTBNDLi = 0; CTBNDLi < CTBNDLnrhs; CTBNDLi++)
    {
      fprintf (stderr, "   $%d = ", CTBNDLi + 1);
      CTBNDL_symbol_print (stderr, CTBNDLrhs[CTBNDLprhs[CTBNDLrule] + CTBNDLi],
		       &(CTBNDLvsp[(CTBNDLi + 1) - (CTBNDLnrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (CTBNDLdebug)				\
    CTBNDL_reduce_print (CTBNDLvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int CTBNDLdebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef CTBNDLstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define CTBNDLstrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
CTBNDLstrlen (const char *CTBNDLstr)
#else
static YYSIZE_T
CTBNDLstrlen (CTBNDLstr)
    const char *CTBNDLstr;
#endif
{
  YYSIZE_T CTBNDLlen;
  for (CTBNDLlen = 0; CTBNDLstr[CTBNDLlen]; CTBNDLlen++)
    continue;
  return CTBNDLlen;
}
#  endif
# endif

# ifndef CTBNDLstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define CTBNDLstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
CTBNDLstpcpy (char *CTBNDLdest, const char *CTBNDLsrc)
#else
static char *
CTBNDLstpcpy (CTBNDLdest, CTBNDLsrc)
    char *CTBNDLdest;
    const char *CTBNDLsrc;
#endif
{
  char *CTBNDLd = CTBNDLdest;
  const char *CTBNDLs = CTBNDLsrc;

  while ((*CTBNDLd++ = *CTBNDLs++) != '\0')
    continue;

  return CTBNDLd - 1;
}
#  endif
# endif

# ifndef CTBNDLtnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for CTBNDLerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from CTBNDLtname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
CTBNDLtnamerr (char *CTBNDLres, const char *CTBNDLstr)
{
  if (*CTBNDLstr == '"')
    {
      YYSIZE_T CTBNDLn = 0;
      char const *CTBNDLp = CTBNDLstr;

      for (;;)
	switch (*++CTBNDLp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++CTBNDLp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (CTBNDLres)
	      CTBNDLres[CTBNDLn] = *CTBNDLp;
	    CTBNDLn++;
	    break;

	  case '"':
	    if (CTBNDLres)
	      CTBNDLres[CTBNDLn] = '\0';
	    return CTBNDLn;
	  }
    do_not_strip_quotes: ;
    }

  if (! CTBNDLres)
    return CTBNDLstrlen (CTBNDLstr);

  return CTBNDLstpcpy (CTBNDLres, CTBNDLstr) - CTBNDLres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
CTBNDLsyntax_error (char *CTBNDLresult, int CTBNDLstate, int CTBNDLchar)
{
  int CTBNDLn = CTBNDLpact[CTBNDLstate];

  if (! (YYPACT_NINF < CTBNDLn && CTBNDLn <= YYLAST))
    return 0;
  else
    {
      int CTBNDLtype = YYTRANSLATE (CTBNDLchar);
      YYSIZE_T CTBNDLsize0 = CTBNDLtnamerr (0, CTBNDLtname[CTBNDLtype]);
      YYSIZE_T CTBNDLsize = CTBNDLsize0;
      YYSIZE_T CTBNDLsize1;
      int CTBNDLsize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *CTBNDLarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int CTBNDLx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *CTBNDLfmt;
      char const *CTBNDLf;
      static char const CTBNDLunexpected[] = "syntax error, unexpected %s";
      static char const CTBNDLexpecting[] = ", expecting %s";
      static char const CTBNDLor[] = " or %s";
      char CTBNDLformat[sizeof CTBNDLunexpected
		    + sizeof CTBNDLexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof CTBNDLor - 1))];
      char const *CTBNDLprefix = CTBNDLexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int CTBNDLxbegin = CTBNDLn < 0 ? -CTBNDLn : 0;

      /* Stay within bounds of both CTBNDLcheck and CTBNDLtname.  */
      int CTBNDLchecklim = YYLAST - CTBNDLn + 1;
      int CTBNDLxend = CTBNDLchecklim < YYNTOKENS ? CTBNDLchecklim : YYNTOKENS;
      int CTBNDLcount = 1;

      CTBNDLarg[0] = CTBNDLtname[CTBNDLtype];
      CTBNDLfmt = CTBNDLstpcpy (CTBNDLformat, CTBNDLunexpected);

      for (CTBNDLx = CTBNDLxbegin; CTBNDLx < CTBNDLxend; ++CTBNDLx)
	if (CTBNDLcheck[CTBNDLx + CTBNDLn] == CTBNDLx && CTBNDLx != YYTERROR)
	  {
	    if (CTBNDLcount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		CTBNDLcount = 1;
		CTBNDLsize = CTBNDLsize0;
		CTBNDLformat[sizeof CTBNDLunexpected - 1] = '\0';
		break;
	      }
	    CTBNDLarg[CTBNDLcount++] = CTBNDLtname[CTBNDLx];
	    CTBNDLsize1 = CTBNDLsize + CTBNDLtnamerr (0, CTBNDLtname[CTBNDLx]);
	    CTBNDLsize_overflow |= (CTBNDLsize1 < CTBNDLsize);
	    CTBNDLsize = CTBNDLsize1;
	    CTBNDLfmt = CTBNDLstpcpy (CTBNDLfmt, CTBNDLprefix);
	    CTBNDLprefix = CTBNDLor;
	  }

      CTBNDLf = YY_(CTBNDLformat);
      CTBNDLsize1 = CTBNDLsize + CTBNDLstrlen (CTBNDLf);
      CTBNDLsize_overflow |= (CTBNDLsize1 < CTBNDLsize);
      CTBNDLsize = CTBNDLsize1;

      if (CTBNDLsize_overflow)
	return YYSIZE_MAXIMUM;

      if (CTBNDLresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *CTBNDLp = CTBNDLresult;
	  int CTBNDLi = 0;
	  while ((*CTBNDLp = *CTBNDLf) != '\0')
	    {
	      if (*CTBNDLp == '%' && CTBNDLf[1] == 's' && CTBNDLi < CTBNDLcount)
		{
		  CTBNDLp += CTBNDLtnamerr (CTBNDLp, CTBNDLarg[CTBNDLi++]);
		  CTBNDLf += 2;
		}
	      else
		{
		  CTBNDLp++;
		  CTBNDLf++;
		}
	    }
	}
      return CTBNDLsize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
CTBNDLdestruct (const char *CTBNDLmsg, int CTBNDLtype, YYSTYPE *CTBNDLvaluep)
#else
static void
CTBNDLdestruct (CTBNDLmsg, CTBNDLtype, CTBNDLvaluep)
    const char *CTBNDLmsg;
    int CTBNDLtype;
    YYSTYPE *CTBNDLvaluep;
#endif
{
  YYUSE (CTBNDLvaluep);

  if (!CTBNDLmsg)
    CTBNDLmsg = "Deleting";
  YY_SYMBOL_PRINT (CTBNDLmsg, CTBNDLtype, CTBNDLvaluep, CTBNDLlocationp);

  switch (CTBNDLtype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int CTBNDLparse (void *YYPARSE_PARAM);
#else
int CTBNDLparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int CTBNDLparse (void);
#else
int CTBNDLparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int CTBNDLchar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE CTBNDLlval;

/* Number of syntax errors so far.  */
int CTBNDLnerrs;



/*----------.
| CTBNDLparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
CTBNDLparse (void *YYPARSE_PARAM)
#else
int
CTBNDLparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
CTBNDLparse (void)
#else
int
CTBNDLparse ()

#endif
#endif
{
  
  int CTBNDLstate;
  int CTBNDLn;
  int CTBNDLresult;
  /* Number of tokens to shift before error messages enabled.  */
  int CTBNDLerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int CTBNDLtoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char CTBNDLmsgbuf[128];
  char *CTBNDLmsg = CTBNDLmsgbuf;
  YYSIZE_T CTBNDLmsg_alloc = sizeof CTBNDLmsgbuf;
#endif

  /* Three stacks and their tools:
     `CTBNDLss': related to states,
     `CTBNDLvs': related to semantic values,
     `CTBNDLls': related to locations.

     Refer to the stacks thru separate pointers, to allow CTBNDLoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  CTBNDLtype_int16 CTBNDLssa[YYINITDEPTH];
  CTBNDLtype_int16 *CTBNDLss = CTBNDLssa;
  CTBNDLtype_int16 *CTBNDLssp;

  /* The semantic value stack.  */
  YYSTYPE CTBNDLvsa[YYINITDEPTH];
  YYSTYPE *CTBNDLvs = CTBNDLvsa;
  YYSTYPE *CTBNDLvsp;



#define YYPOPSTACK(N)   (CTBNDLvsp -= (N), CTBNDLssp -= (N))

  YYSIZE_T CTBNDLstacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE CTBNDLval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int CTBNDLlen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  CTBNDLstate = 0;
  CTBNDLerrstatus = 0;
  CTBNDLnerrs = 0;
  CTBNDLchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  CTBNDLssp = CTBNDLss;
  CTBNDLvsp = CTBNDLvs;

  goto CTBNDLsetstate;

/*------------------------------------------------------------.
| CTBNDLnewstate -- Push a new state, which is found in CTBNDLstate.  |
`------------------------------------------------------------*/
 CTBNDLnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  CTBNDLssp++;

 CTBNDLsetstate:
  *CTBNDLssp = CTBNDLstate;

  if (CTBNDLss + CTBNDLstacksize - 1 <= CTBNDLssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T CTBNDLsize = CTBNDLssp - CTBNDLss + 1;

#ifdef CTBNDLoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *CTBNDLvs1 = CTBNDLvs;
	CTBNDLtype_int16 *CTBNDLss1 = CTBNDLss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if CTBNDLoverflow is a macro.  */
	CTBNDLoverflow (YY_("memory exhausted"),
		    &CTBNDLss1, CTBNDLsize * sizeof (*CTBNDLssp),
		    &CTBNDLvs1, CTBNDLsize * sizeof (*CTBNDLvsp),

		    &CTBNDLstacksize);

	CTBNDLss = CTBNDLss1;
	CTBNDLvs = CTBNDLvs1;
      }
#else /* no CTBNDLoverflow */
# ifndef YYSTACK_RELOCATE
      goto CTBNDLexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= CTBNDLstacksize)
	goto CTBNDLexhaustedlab;
      CTBNDLstacksize *= 2;
      if (YYMAXDEPTH < CTBNDLstacksize)
	CTBNDLstacksize = YYMAXDEPTH;

      {
	CTBNDLtype_int16 *CTBNDLss1 = CTBNDLss;
	union CTBNDLalloc *CTBNDLptr =
	  (union CTBNDLalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (CTBNDLstacksize));
	if (! CTBNDLptr)
	  goto CTBNDLexhaustedlab;
	YYSTACK_RELOCATE (CTBNDLss);
	YYSTACK_RELOCATE (CTBNDLvs);

#  undef YYSTACK_RELOCATE
	if (CTBNDLss1 != CTBNDLssa)
	  YYSTACK_FREE (CTBNDLss1);
      }
# endif
#endif /* no CTBNDLoverflow */

      CTBNDLssp = CTBNDLss + CTBNDLsize - 1;
      CTBNDLvsp = CTBNDLvs + CTBNDLsize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) CTBNDLstacksize));

      if (CTBNDLss + CTBNDLstacksize - 1 <= CTBNDLssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", CTBNDLstate));

  goto CTBNDLbackup;

/*-----------.
| CTBNDLbackup.  |
`-----------*/
CTBNDLbackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  CTBNDLn = CTBNDLpact[CTBNDLstate];
  if (CTBNDLn == YYPACT_NINF)
    goto CTBNDLdefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (CTBNDLchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      CTBNDLchar = YYLEX;
    }

  if (CTBNDLchar <= YYEOF)
    {
      CTBNDLchar = CTBNDLtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      CTBNDLtoken = YYTRANSLATE (CTBNDLchar);
      YY_SYMBOL_PRINT ("Next token is", CTBNDLtoken, &CTBNDLlval, &CTBNDLlloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  CTBNDLn += CTBNDLtoken;
  if (CTBNDLn < 0 || YYLAST < CTBNDLn || CTBNDLcheck[CTBNDLn] != CTBNDLtoken)
    goto CTBNDLdefault;
  CTBNDLn = CTBNDLtable[CTBNDLn];
  if (CTBNDLn <= 0)
    {
      if (CTBNDLn == 0 || CTBNDLn == YYTABLE_NINF)
	goto CTBNDLerrlab;
      CTBNDLn = -CTBNDLn;
      goto CTBNDLreduce;
    }

  if (CTBNDLn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (CTBNDLerrstatus)
    CTBNDLerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", CTBNDLtoken, &CTBNDLlval, &CTBNDLlloc);

  /* Discard the shifted token unless it is eof.  */
  if (CTBNDLchar != YYEOF)
    CTBNDLchar = YYEMPTY;

  CTBNDLstate = CTBNDLn;
  *++CTBNDLvsp = CTBNDLlval;

  goto CTBNDLnewstate;


/*-----------------------------------------------------------.
| CTBNDLdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
CTBNDLdefault:
  CTBNDLn = CTBNDLdefact[CTBNDLstate];
  if (CTBNDLn == 0)
    goto CTBNDLerrlab;
  goto CTBNDLreduce;


/*-----------------------------.
| CTBNDLreduce -- Do a reduction.  |
`-----------------------------*/
CTBNDLreduce:
  /* CTBNDLn is the number of a rule to reduce with.  */
  CTBNDLlen = CTBNDLr2[CTBNDLn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  CTBNDLval = CTBNDLvsp[1-CTBNDLlen];


  YY_REDUCE_PRINT (CTBNDLn);
  switch (CTBNDLn)
    {
        case 2:
#line 99 "BooleanGrammar.y"
    {
;}
    break;

  case 3:
#line 102 "BooleanGrammar.y"
    {
;}
    break;

  case 4:
#line 107 "BooleanGrammar.y"
    {
  NodeDecl* truc = new NodeDecl((CTBNDLvsp[(2) - (5)].str), (CTBNDLvsp[(4) - (5)].node_decl_item_list));
  free((CTBNDLvsp[(2) - (5)].str));
  for (std::vector<NodeDeclItem*>::iterator it = (CTBNDLvsp[(4) - (5)].node_decl_item_list)->begin(); it != (CTBNDLvsp[(4) - (5)].node_decl_item_list)->end(); ++it) {
    delete *it;
  }
  delete (CTBNDLvsp[(4) - (5)].node_decl_item_list);
  delete truc;
;}
    break;

  case 5:
#line 117 "BooleanGrammar.y"
    {
  NodeDecl* truc = new NodeDecl((CTBNDLvsp[(2) - (4)].str), NULL);
  free((CTBNDLvsp[(2) - (4)].str));
  delete truc;
;}
    break;

  case 6:
#line 123 "BooleanGrammar.y"
    {
  NodeDeclItem* decl_item = new NodeDeclItem("logic", (CTBNDLvsp[(3) - (4)].expr));
  std::vector<NodeDeclItem*>* decl_item_v = new std::vector<NodeDeclItem*>();
  decl_item_v->push_back(decl_item);

  NodeDecl* truc = new NodeDecl((CTBNDLvsp[(1) - (4)].str), decl_item_v);
  free((CTBNDLvsp[(1) - (4)].str));
  for (std::vector<NodeDeclItem*>::iterator it = decl_item_v->begin(); it != decl_item_v->end(); ++it) {
    delete *it;
  }
  delete decl_item_v;
  delete truc;
;}
    break;

  case 7:
#line 139 "BooleanGrammar.y"
    {;}
    break;

  case 8:
#line 141 "BooleanGrammar.y"
    {;}
    break;

  case 9:
#line 145 "BooleanGrammar.y"
    {
  (CTBNDLval.node_decl_item_list) = new std::vector<NodeDeclItem*>();
  (CTBNDLval.node_decl_item_list)->push_back((CTBNDLvsp[(1) - (1)].node_decl_item));
;}
    break;

  case 10:
#line 150 "BooleanGrammar.y"
    {
  (CTBNDLvsp[(1) - (2)].node_decl_item_list)->push_back((CTBNDLvsp[(2) - (2)].node_decl_item));
  (CTBNDLval.node_decl_item_list) = (CTBNDLvsp[(1) - (2)].node_decl_item_list);
;}
    break;

  case 11:
#line 157 "BooleanGrammar.y"
    {
  (CTBNDLval.node_decl_item) = new NodeDeclItem((CTBNDLvsp[(1) - (4)].str), (CTBNDLvsp[(3) - (4)].expr));
  free((CTBNDLvsp[(1) - (4)].str));
;}
    break;

  case 12:
#line 162 "BooleanGrammar.y"
    {
  (CTBNDLval.node_decl_item) = new NodeDeclItem((CTBNDLvsp[(1) - (4)].str), (CTBNDLvsp[(3) - (4)].str));
  free((CTBNDLvsp[(1) - (4)].str));
;}
    break;

  case 13:
#line 169 "BooleanGrammar.y"
    {
  Node* node = current_network->getOrMakeNode((CTBNDLvsp[(1) - (1)].str));
  (CTBNDLval.expr) = new NodeExpression(node);
  free((CTBNDLvsp[(1) - (1)].str));
;}
    break;

  case 14:
#line 175 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new AliasExpression((CTBNDLvsp[(2) - (2)].str));
  free((CTBNDLvsp[(2) - (2)].str));
;}
    break;

  case 15:
#line 180 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new SymbolExpression(current_network->getSymbolTable(), current_network->getSymbolTable()->getOrMakeSymbol((CTBNDLvsp[(1) - (1)].str)));
  free((CTBNDLvsp[(1) - (1)].str));
;}
    break;

  case 16:
#line 185 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new ConstantExpression((CTBNDLvsp[(1) - (1)].l));
;}
    break;

  case 17:
#line 189 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new ConstantExpression((CTBNDLvsp[(1) - (1)].d));
;}
    break;

  case 18:
#line 193 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new ParenthesisExpression((CTBNDLvsp[(2) - (3)].expr));
;}
    break;

  case 19:
#line 199 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 20:
#line 203 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new FuncCallExpression((CTBNDLvsp[(1) - (4)].str), (CTBNDLvsp[(3) - (4)].arg_list));
  free((CTBNDLvsp[(1) - (4)].str));
;}
    break;

  case 21:
#line 208 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new FuncCallExpression((CTBNDLvsp[(1) - (3)].str), NULL);
  free((CTBNDLvsp[(1) - (3)].str));
;}
    break;

  case 22:
#line 215 "BooleanGrammar.y"
    {
  (CTBNDLval.arg_list) = new ArgumentList();
  (CTBNDLval.arg_list)->push_back((CTBNDLvsp[(1) - (1)].expr));
;}
    break;

  case 23:
#line 220 "BooleanGrammar.y"
    {
  (CTBNDLval.arg_list) = (CTBNDLvsp[(1) - (3)].arg_list);
  (CTBNDLval.arg_list)->push_back((CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 24:
#line 227 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 25:
#line 231 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(2) - (2)].expr);
;}
    break;

  case 26:
#line 235 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new SubExpression(new ConstantExpression(0.0), (CTBNDLvsp[(2) - (2)].expr));
;}
    break;

  case 27:
#line 239 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new NotLogicalExpression((CTBNDLvsp[(2) - (2)].expr));
;}
    break;

  case 28:
#line 243 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new NotLogicalExpression((CTBNDLvsp[(2) - (2)].expr));
;}
    break;

  case 29:
#line 249 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 30:
#line 253 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new MulExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 31:
#line 257 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new DivExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 32:
#line 263 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 33:
#line 267 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new AddExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 34:
#line 271 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new SubExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 35:
#line 277 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 36:
#line 281 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new LetterExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 37:
#line 285 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new GreaterExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 38:
#line 289 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new LetterOrEqualExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 39:
#line 293 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new GreaterOrEqualExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 40:
#line 299 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 41:
#line 303 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new EqualExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 42:
#line 307 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new NotEqualExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 43:
#line 313 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 44:
#line 317 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new AndLogicalExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 45:
#line 323 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 46:
#line 327 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new OrLogicalExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 47:
#line 333 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 48:
#line 337 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new XorLogicalExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 49:
#line 341 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new XorLogicalExpression((CTBNDLvsp[(1) - (3)].expr), (CTBNDLvsp[(3) - (3)].expr));
;}
    break;

  case 50:
#line 347 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;

  case 51:
#line 351 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = new CondExpression((CTBNDLvsp[(1) - (5)].expr), (CTBNDLvsp[(3) - (5)].expr), (CTBNDLvsp[(5) - (5)].expr));
;}
    break;

  case 52:
#line 357 "BooleanGrammar.y"
    {
  (CTBNDLval.expr) = (CTBNDLvsp[(1) - (1)].expr);
;}
    break;


/* Line 1267 of yacc.c.  */
#line 1868 "BooleanGrammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", CTBNDLr1[CTBNDLn], &CTBNDLval, &CTBNDLloc);

  YYPOPSTACK (CTBNDLlen);
  CTBNDLlen = 0;
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);

  *++CTBNDLvsp = CTBNDLval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  CTBNDLn = CTBNDLr1[CTBNDLn];

  CTBNDLstate = CTBNDLpgoto[CTBNDLn - YYNTOKENS] + *CTBNDLssp;
  if (0 <= CTBNDLstate && CTBNDLstate <= YYLAST && CTBNDLcheck[CTBNDLstate] == *CTBNDLssp)
    CTBNDLstate = CTBNDLtable[CTBNDLstate];
  else
    CTBNDLstate = CTBNDLdefgoto[CTBNDLn - YYNTOKENS];

  goto CTBNDLnewstate;


/*------------------------------------.
| CTBNDLerrlab -- here on detecting error |
`------------------------------------*/
CTBNDLerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!CTBNDLerrstatus)
    {
      ++CTBNDLnerrs;
#if ! YYERROR_VERBOSE
      CTBNDLerror (YY_("syntax error"));
#else
      {
	YYSIZE_T CTBNDLsize = CTBNDLsyntax_error (0, CTBNDLstate, CTBNDLchar);
	if (CTBNDLmsg_alloc < CTBNDLsize && CTBNDLmsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T CTBNDLalloc = 2 * CTBNDLsize;
	    if (! (CTBNDLsize <= CTBNDLalloc && CTBNDLalloc <= YYSTACK_ALLOC_MAXIMUM))
	      CTBNDLalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (CTBNDLmsg != CTBNDLmsgbuf)
	      YYSTACK_FREE (CTBNDLmsg);
	    CTBNDLmsg = (char *) YYSTACK_ALLOC (CTBNDLalloc);
	    if (CTBNDLmsg)
	      CTBNDLmsg_alloc = CTBNDLalloc;
	    else
	      {
		CTBNDLmsg = CTBNDLmsgbuf;
		CTBNDLmsg_alloc = sizeof CTBNDLmsgbuf;
	      }
	  }

	if (0 < CTBNDLsize && CTBNDLsize <= CTBNDLmsg_alloc)
	  {
	    (void) CTBNDLsyntax_error (CTBNDLmsg, CTBNDLstate, CTBNDLchar);
	    CTBNDLerror (CTBNDLmsg);
	  }
	else
	  {
	    CTBNDLerror (YY_("syntax error"));
	    if (CTBNDLsize != 0)
	      goto CTBNDLexhaustedlab;
	  }
      }
#endif
    }



  if (CTBNDLerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (CTBNDLchar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (CTBNDLchar == YYEOF)
	    YYABORT;
	}
      else
	{
	  CTBNDLdestruct ("Error: discarding",
		      CTBNDLtoken, &CTBNDLlval);
	  CTBNDLchar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto CTBNDLerrlab1;


/*---------------------------------------------------.
| CTBNDLerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
CTBNDLerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label CTBNDLerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto CTBNDLerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (CTBNDLlen);
  CTBNDLlen = 0;
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
  CTBNDLstate = *CTBNDLssp;
  goto CTBNDLerrlab1;


/*-------------------------------------------------------------.
| CTBNDLerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
CTBNDLerrlab1:
  CTBNDLerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      CTBNDLn = CTBNDLpact[CTBNDLstate];
      if (CTBNDLn != YYPACT_NINF)
	{
	  CTBNDLn += YYTERROR;
	  if (0 <= CTBNDLn && CTBNDLn <= YYLAST && CTBNDLcheck[CTBNDLn] == YYTERROR)
	    {
	      CTBNDLn = CTBNDLtable[CTBNDLn];
	      if (0 < CTBNDLn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (CTBNDLssp == CTBNDLss)
	YYABORT;


      CTBNDLdestruct ("Error: popping",
		  CTBNDLstos[CTBNDLstate], CTBNDLvsp);
      YYPOPSTACK (1);
      CTBNDLstate = *CTBNDLssp;
      YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
    }

  if (CTBNDLn == YYFINAL)
    YYACCEPT;

  *++CTBNDLvsp = CTBNDLlval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", CTBNDLstos[CTBNDLn], CTBNDLvsp, CTBNDLlsp);

  CTBNDLstate = CTBNDLn;
  goto CTBNDLnewstate;


/*-------------------------------------.
| CTBNDLacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
CTBNDLacceptlab:
  CTBNDLresult = 0;
  goto CTBNDLreturn;

/*-----------------------------------.
| CTBNDLabortlab -- YYABORT comes here.  |
`-----------------------------------*/
CTBNDLabortlab:
  CTBNDLresult = 1;
  goto CTBNDLreturn;

#ifndef CTBNDLoverflow
/*-------------------------------------------------.
| CTBNDLexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
CTBNDLexhaustedlab:
  CTBNDLerror (YY_("memory exhausted"));
  CTBNDLresult = 2;
  /* Fall through.  */
#endif

CTBNDLreturn:
  if (CTBNDLchar != YYEOF && CTBNDLchar != YYEMPTY)
     CTBNDLdestruct ("Cleanup: discarding lookahead",
		 CTBNDLtoken, &CTBNDLlval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (CTBNDLlen);
  YY_STACK_PRINT (CTBNDLss, CTBNDLssp);
  while (CTBNDLssp != CTBNDLss)
    {
      CTBNDLdestruct ("Cleanup: popping",
		  CTBNDLstos[*CTBNDLssp], CTBNDLvsp);
      YYPOPSTACK (1);
    }
#ifndef CTBNDLoverflow
  if (CTBNDLss != CTBNDLssa)
    YYSTACK_FREE (CTBNDLss);
#endif
#if YYERROR_VERBOSE
  if (CTBNDLmsg != CTBNDLmsgbuf)
    YYSTACK_FREE (CTBNDLmsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (CTBNDLresult);
}


#line 366 "BooleanGrammar.y"


#include "lex.CTBNDL.cc"

void set_current_network(Network* network)
{
  current_network = network;
}

Network* get_current_network()
{
  return current_network;
}

