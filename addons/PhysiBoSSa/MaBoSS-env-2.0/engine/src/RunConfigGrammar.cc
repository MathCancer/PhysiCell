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

/* All symbols defined below should begin with RC or YY, to avoid
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
   enum RCtokentype {
     VARIABLE = 258,
     SYMBOL = 259,
     DOUBLE = 260,
     INTEGER = 261,
     LOGAND = 262,
     LOGOR = 263,
     LOGXOR = 264,
     LOGNOT = 265,
     EQUAL = 266,
     NOT_EQUAL = 267,
     NODE = 268,
     GTEQ = 269,
     LTEQ = 270
   };
#endif
/* Tokens.  */
#define VARIABLE 258
#define SYMBOL 259
#define DOUBLE 260
#define INTEGER 261
#define LOGAND 262
#define LOGOR 263
#define LOGXOR 264
#define LOGNOT 265
#define EQUAL 266
#define NOT_EQUAL 267
#define NODE 268
#define GTEQ 269
#define LTEQ 270




/* Copy the first part of user declarations.  */
#line 2 "RunConfigGrammar.y"

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
     RunConfigGrammar.l

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
     updated to add extended initial conditions: October 2014
*/

#define _POSIX_SOURCE 1
#include "BooleanNetwork.h"
#include "RunConfig.h"

extern int RClex();

static void RCerror(const char *s);
static Network* network;
static RunConfig* config;

extern std::string RC_error_head();


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
#line 66 "RunConfigGrammar.y"
{
  Expression* expr;
  char* str;
  double d;
  long long l;
  std::vector<const Node*>* node_list;
  std::vector<Expression*>* expr_list;
  IStateGroup::ProbaIState* istate_expr;
  std::vector<IStateGroup::ProbaIState*>* istate_expr_list;
  ArgumentList* arg_list;

}
/* Line 193 of yacc.c.  */
#line 203 "RunConfigGrammar.tab.c"
	YYSTYPE;
# define RCstype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 216 "RunConfigGrammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 RCtype_uint8;
#else
typedef unsigned char RCtype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 RCtype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char RCtype_int8;
#else
typedef short int RCtype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 RCtype_uint16;
#else
typedef unsigned short int RCtype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 RCtype_int16;
#else
typedef short int RCtype_int16;
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

#if ! defined RCoverflow || YYERROR_VERBOSE

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
#endif /* ! defined RCoverflow || YYERROR_VERBOSE */


#if (! defined RCoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union RCalloc
{
  RCtype_int16 RCss;
  YYSTYPE RCvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union RCalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (RCtype_int16) + sizeof (YYSTYPE)) \
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
	  YYSIZE_T RCi;				\
	  for (RCi = 0; RCi < (Count); RCi++)	\
	    (To)[RCi] = (From)[RCi];		\
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
	YYSIZE_T RCnewbytes;						\
	YYCOPY (&RCptr->Stack, Stack, RCsize);				\
	Stack = &RCptr->Stack;						\
	RCnewbytes = RCstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	RCptr += RCnewbytes / sizeof (*RCptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNRULES -- Number of states.  */
#define YYNSTATES  109

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? RCtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const RCtype_uint8 RCtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,     2,     2,     2,     2,     2,
      22,    23,    27,    24,    21,    25,    18,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,    16,
      29,    17,    30,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,    20,    31,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const RCtype_uint8 RCprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    21,
      28,    35,    39,    41,    45,    47,    51,    56,    58,    62,
      67,    69,    71,    73,    77,    79,    83,    85,    90,    94,
      96,    99,   102,   105,   108,   110,   114,   118,   120,   124,
     128,   130,   134,   138,   142,   146,   148,   152,   156,   158,
     162,   164,   168,   170,   174,   178,   180,   186
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const RCtype_int8 RCrhs[] =
{
      35,     0,    -1,    36,    -1,    35,    36,    -1,    44,    -1,
      37,    -1,    38,    -1,    16,    -1,     4,    17,    57,    16,
      -1,     4,    18,     4,    17,    57,    16,    -1,    39,    18,
       4,    17,    41,    16,    -1,    19,    40,    20,    -1,     4,
      -1,    40,    21,     4,    -1,    42,    -1,    41,    21,    42,
      -1,    45,    19,    43,    20,    -1,    45,    -1,    43,    21,
      45,    -1,     3,    17,    57,    16,    -1,     6,    -1,     5,
      -1,     3,    -1,    22,    57,    23,    -1,    57,    -1,    46,
      21,    57,    -1,    45,    -1,     4,    22,    46,    23,    -1,
       4,    22,    23,    -1,    47,    -1,    24,    48,    -1,    25,
      48,    -1,    26,    48,    -1,    10,    48,    -1,    48,    -1,
      49,    27,    48,    -1,    49,    28,    48,    -1,    49,    -1,
      50,    24,    49,    -1,    50,    25,    49,    -1,    50,    -1,
      51,    29,    50,    -1,    51,    30,    50,    -1,    51,    15,
      50,    -1,    51,    14,    50,    -1,    51,    -1,    52,    11,
      51,    -1,    52,    12,    51,    -1,    52,    -1,    53,     7,
      52,    -1,    53,    -1,    54,     8,    53,    -1,    54,    -1,
      55,     9,    54,    -1,    55,    31,    54,    -1,    55,    -1,
      55,    32,    57,    33,    56,    -1,    56,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const RCtype_uint16 RCrline[] =
{
       0,   107,   107,   110,   115,   116,   117,   118,   121,   131,
     162,   177,   183,   189,   196,   201,   208,   219,   224,   231,
     241,   245,   249,   254,   260,   265,   273,   277,   282,   290,
     294,   298,   302,   306,   312,   316,   320,   326,   330,   334,
     340,   344,   348,   352,   356,   362,   366,   370,   376,   380,
     386,   390,   396,   400,   404,   410,   414,   420
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const RCtname[] =
{
  "$end", "error", "$undefined", "VARIABLE", "SYMBOL", "DOUBLE",
  "INTEGER", "LOGAND", "LOGOR", "LOGXOR", "LOGNOT", "EQUAL", "NOT_EQUAL",
  "NODE", "GTEQ", "LTEQ", "';'", "'='", "'.'", "'['", "']'", "','", "'('",
  "')'", "'+'", "'-'", "'!'", "'*'", "'/'", "'<'", "'>'", "'^'", "'?'",
  "':'", "$accept", "translation_unit", "decl", "runconfig_decl",
  "node_attr_decl", "symbol_istate_list", "symbol_list",
  "istate_expression_list", "istate_expression", "expression_list",
  "var_decl", "primary_expression", "argument_expression_list",
  "postfix_expression", "unary_expression", "multiplicative_expression",
  "additive_expression", "relational_expression", "equality_expression",
  "logical_and_expression", "logical_or_expression",
  "logical_xor_expression", "conditional_expression", "expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const RCtype_uint16 RCtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    59,    61,    46,    91,
      93,    44,    40,    41,    43,    45,    33,    42,    47,    60,
      62,    94,    63,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const RCtype_uint8 RCr1[] =
{
       0,    34,    35,    35,    36,    36,    36,    36,    37,    38,
      38,    39,    40,    40,    41,    41,    42,    43,    43,    44,
      45,    45,    45,    45,    46,    46,    47,    47,    47,    48,
      48,    48,    48,    48,    49,    49,    49,    50,    50,    50,
      51,    51,    51,    51,    51,    52,    52,    52,    53,    53,
      54,    54,    55,    55,    55,    56,    56,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const RCtype_uint8 RCr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     6,
       6,     3,     1,     3,     1,     3,     4,     1,     3,     4,
       1,     1,     1,     3,     1,     3,     1,     4,     3,     1,
       2,     2,     2,     2,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     5,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const RCtype_uint8 RCdefact[] =
{
       0,     0,     0,     7,     0,     0,     2,     5,     6,     0,
       4,     0,     0,     0,    12,     0,     1,     3,     0,    22,
       0,    21,    20,     0,     0,     0,     0,     0,    26,    29,
      34,    37,    40,    45,    48,    50,    52,    55,    57,     0,
       0,     0,    11,     0,     0,     0,    33,     0,    30,    31,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     8,     0,    13,
       0,    28,     0,    24,    23,    35,    36,    38,    39,    44,
      43,    41,    42,    46,    47,    49,    51,    53,    54,     0,
       0,     0,    14,     0,     0,    27,     0,     9,    10,     0,
       0,    25,    56,    15,     0,    17,    16,     0,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const RCtype_int8 RCdefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    15,    91,    92,   104,
      10,    28,    72,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -70
static const RCtype_int8 RCpact[] =
{
      45,    -6,    -3,   -70,    46,    13,   -70,   -70,   -70,     8,
     -70,    41,    41,    68,   -70,    16,   -70,   -70,    83,   -70,
       6,   -70,   -70,    41,    41,    41,    41,    41,   -70,   -70,
     -70,    42,    50,   -11,    65,    55,    80,    -7,   -70,    73,
      74,    75,   -70,    87,    76,    17,   -70,    71,   -70,   -70,
     -70,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,   -70,   -70,    41,   -70,
      49,   -70,   -16,   -70,   -70,   -70,   -70,    42,    42,    50,
      50,    50,    50,   -11,   -11,    65,    55,    80,    80,    62,
      81,    52,   -70,    77,    41,   -70,    41,   -70,   -70,    49,
      49,   -70,   -70,   -70,    58,   -70,   -70,    49,   -70
};

/* YYPGOTO[NTERM-NUM].  */
static const RCtype_int8 RCpgoto[] =
{
     -70,   -70,    93,   -70,   -70,   -70,   -70,   -70,     0,   -70,
     -70,   -69,   -70,   -70,   -17,    27,     2,    24,    39,    40,
      22,   -70,     5,   -12
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const RCtype_uint8 RCtable[] =
{
      40,    93,    63,    55,    56,    94,    46,    95,    48,    49,
      50,    11,    47,    16,    12,    13,     1,     2,    57,    58,
      19,    20,    21,    22,    64,    65,    18,    23,    45,     3,
      93,   105,     4,    73,    75,    76,    42,    43,   108,    24,
      71,    25,    26,    27,    19,    20,    21,    22,     1,     2,
      14,    23,    19,    89,    21,    22,    90,    79,    80,    81,
      82,     3,    61,    24,     4,    25,    26,    27,    98,    51,
      52,    24,    41,    99,    53,    54,    59,    60,   106,   107,
      77,    78,   101,    83,    84,    87,    88,    44,    62,    66,
      67,    69,    68,    70,    74,    96,   100,    97,    17,   103,
      85,   102,    86
};

static const RCtype_uint8 RCcheck[] =
{
      12,    70,     9,    14,    15,    21,    23,    23,    25,    26,
      27,    17,    24,     0,    17,    18,     3,     4,    29,    30,
       3,     4,     5,     6,    31,    32,    18,    10,    22,    16,
      99,   100,    19,    45,    51,    52,    20,    21,   107,    22,
      23,    24,    25,    26,     3,     4,     5,     6,     3,     4,
       4,    10,     3,    65,     5,     6,    68,    55,    56,    57,
      58,    16,     7,    22,    19,    24,    25,    26,    16,    27,
      28,    22,     4,    21,    24,    25,    11,    12,    20,    21,
      53,    54,    94,    59,    60,    63,    64,     4,     8,    16,
      16,     4,    17,    17,    23,    33,    19,    16,     5,    99,
      61,    96,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const RCtype_uint8 RCstos[] =
{
       0,     3,     4,    16,    19,    35,    36,    37,    38,    39,
      44,    17,    17,    18,     4,    40,     0,    36,    18,     3,
       4,     5,     6,    10,    22,    24,    25,    26,    45,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      57,     4,    20,    21,     4,    22,    48,    57,    48,    48,
      48,    27,    28,    24,    25,    14,    15,    29,    30,    11,
      12,     7,     8,     9,    31,    32,    16,    16,    17,     4,
      17,    23,    46,    57,    23,    48,    48,    49,    49,    50,
      50,    50,    50,    51,    51,    52,    53,    54,    54,    57,
      57,    41,    42,    45,    21,    23,    33,    16,    16,    21,
      19,    57,    56,    42,    43,    45,    20,    21,    45
};

#define RCerrok		(RCerrstatus = 0)
#define RCclearin	(RCchar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto RCacceptlab
#define YYABORT		goto RCabortlab
#define YYERROR		goto RCerrorlab


/* Like YYERROR except do call RCerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto RCerrlab

#define YYRECOVERING()  (!!RCerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (RCchar == YYEMPTY && RClen == 1)				\
    {								\
      RCchar = (Token);						\
      RClval = (Value);						\
      RCtoken = YYTRANSLATE (RCchar);				\
      YYPOPSTACK (1);						\
      goto RCbackup;						\
    }								\
  else								\
    {								\
      RCerror (YY_("syntax error: cannot back up")); \
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


/* YYLEX -- calling `RClex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX RClex (YYLEX_PARAM)
#else
# define YYLEX RClex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (RCdebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (RCdebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      RC_symbol_print (stderr,						  \
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
RC_symbol_value_print (FILE *RCoutput, int RCtype, YYSTYPE const * const RCvaluep)
#else
static void
RC_symbol_value_print (RCoutput, RCtype, RCvaluep)
    FILE *RCoutput;
    int RCtype;
    YYSTYPE const * const RCvaluep;
#endif
{
  if (!RCvaluep)
    return;
# ifdef YYPRINT
  if (RCtype < YYNTOKENS)
    YYPRINT (RCoutput, RCtoknum[RCtype], *RCvaluep);
# else
  YYUSE (RCoutput);
# endif
  switch (RCtype)
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
RC_symbol_print (FILE *RCoutput, int RCtype, YYSTYPE const * const RCvaluep)
#else
static void
RC_symbol_print (RCoutput, RCtype, RCvaluep)
    FILE *RCoutput;
    int RCtype;
    YYSTYPE const * const RCvaluep;
#endif
{
  if (RCtype < YYNTOKENS)
    YYFPRINTF (RCoutput, "token %s (", RCtname[RCtype]);
  else
    YYFPRINTF (RCoutput, "nterm %s (", RCtname[RCtype]);

  RC_symbol_value_print (RCoutput, RCtype, RCvaluep);
  YYFPRINTF (RCoutput, ")");
}

/*------------------------------------------------------------------.
| RC_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
RC_stack_print (RCtype_int16 *bottom, RCtype_int16 *top)
#else
static void
RC_stack_print (bottom, top)
    RCtype_int16 *bottom;
    RCtype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (RCdebug)							\
    RC_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
RC_reduce_print (YYSTYPE *RCvsp, int RCrule)
#else
static void
RC_reduce_print (RCvsp, RCrule)
    YYSTYPE *RCvsp;
    int RCrule;
#endif
{
  int RCnrhs = RCr2[RCrule];
  int RCi;
  unsigned long int RClno = RCrline[RCrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     RCrule - 1, RClno);
  /* The symbols being reduced.  */
  for (RCi = 0; RCi < RCnrhs; RCi++)
    {
      fprintf (stderr, "   $%d = ", RCi + 1);
      RC_symbol_print (stderr, RCrhs[RCprhs[RCrule] + RCi],
		       &(RCvsp[(RCi + 1) - (RCnrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (RCdebug)				\
    RC_reduce_print (RCvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int RCdebug;
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

# ifndef RCstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define RCstrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
RCstrlen (const char *RCstr)
#else
static YYSIZE_T
RCstrlen (RCstr)
    const char *RCstr;
#endif
{
  YYSIZE_T RClen;
  for (RClen = 0; RCstr[RClen]; RClen++)
    continue;
  return RClen;
}
#  endif
# endif

# ifndef RCstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define RCstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
RCstpcpy (char *RCdest, const char *RCsrc)
#else
static char *
RCstpcpy (RCdest, RCsrc)
    char *RCdest;
    const char *RCsrc;
#endif
{
  char *RCd = RCdest;
  const char *RCs = RCsrc;

  while ((*RCd++ = *RCs++) != '\0')
    continue;

  return RCd - 1;
}
#  endif
# endif

# ifndef RCtnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for RCerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from RCtname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
RCtnamerr (char *RCres, const char *RCstr)
{
  if (*RCstr == '"')
    {
      YYSIZE_T RCn = 0;
      char const *RCp = RCstr;

      for (;;)
	switch (*++RCp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++RCp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (RCres)
	      RCres[RCn] = *RCp;
	    RCn++;
	    break;

	  case '"':
	    if (RCres)
	      RCres[RCn] = '\0';
	    return RCn;
	  }
    do_not_strip_quotes: ;
    }

  if (! RCres)
    return RCstrlen (RCstr);

  return RCstpcpy (RCres, RCstr) - RCres;
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
RCsyntax_error (char *RCresult, int RCstate, int RCchar)
{
  int RCn = RCpact[RCstate];

  if (! (YYPACT_NINF < RCn && RCn <= YYLAST))
    return 0;
  else
    {
      int RCtype = YYTRANSLATE (RCchar);
      YYSIZE_T RCsize0 = RCtnamerr (0, RCtname[RCtype]);
      YYSIZE_T RCsize = RCsize0;
      YYSIZE_T RCsize1;
      int RCsize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *RCarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int RCx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *RCfmt;
      char const *RCf;
      static char const RCunexpected[] = "syntax error, unexpected %s";
      static char const RCexpecting[] = ", expecting %s";
      static char const RCor[] = " or %s";
      char RCformat[sizeof RCunexpected
		    + sizeof RCexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof RCor - 1))];
      char const *RCprefix = RCexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int RCxbegin = RCn < 0 ? -RCn : 0;

      /* Stay within bounds of both RCcheck and RCtname.  */
      int RCchecklim = YYLAST - RCn + 1;
      int RCxend = RCchecklim < YYNTOKENS ? RCchecklim : YYNTOKENS;
      int RCcount = 1;

      RCarg[0] = RCtname[RCtype];
      RCfmt = RCstpcpy (RCformat, RCunexpected);

      for (RCx = RCxbegin; RCx < RCxend; ++RCx)
	if (RCcheck[RCx + RCn] == RCx && RCx != YYTERROR)
	  {
	    if (RCcount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		RCcount = 1;
		RCsize = RCsize0;
		RCformat[sizeof RCunexpected - 1] = '\0';
		break;
	      }
	    RCarg[RCcount++] = RCtname[RCx];
	    RCsize1 = RCsize + RCtnamerr (0, RCtname[RCx]);
	    RCsize_overflow |= (RCsize1 < RCsize);
	    RCsize = RCsize1;
	    RCfmt = RCstpcpy (RCfmt, RCprefix);
	    RCprefix = RCor;
	  }

      RCf = YY_(RCformat);
      RCsize1 = RCsize + RCstrlen (RCf);
      RCsize_overflow |= (RCsize1 < RCsize);
      RCsize = RCsize1;

      if (RCsize_overflow)
	return YYSIZE_MAXIMUM;

      if (RCresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *RCp = RCresult;
	  int RCi = 0;
	  while ((*RCp = *RCf) != '\0')
	    {
	      if (*RCp == '%' && RCf[1] == 's' && RCi < RCcount)
		{
		  RCp += RCtnamerr (RCp, RCarg[RCi++]);
		  RCf += 2;
		}
	      else
		{
		  RCp++;
		  RCf++;
		}
	    }
	}
      return RCsize;
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
RCdestruct (const char *RCmsg, int RCtype, YYSTYPE *RCvaluep)
#else
static void
RCdestruct (RCmsg, RCtype, RCvaluep)
    const char *RCmsg;
    int RCtype;
    YYSTYPE *RCvaluep;
#endif
{
  YYUSE (RCvaluep);

  if (!RCmsg)
    RCmsg = "Deleting";
  YY_SYMBOL_PRINT (RCmsg, RCtype, RCvaluep, RClocationp);

  switch (RCtype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int RCparse (void *YYPARSE_PARAM);
#else
int RCparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int RCparse (void);
#else
int RCparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int RCchar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE RClval;

/* Number of syntax errors so far.  */
int RCnerrs;



/*----------.
| RCparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
RCparse (void *YYPARSE_PARAM)
#else
int
RCparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
RCparse (void)
#else
int
RCparse ()

#endif
#endif
{
  
  int RCstate;
  int RCn;
  int RCresult;
  /* Number of tokens to shift before error messages enabled.  */
  int RCerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int RCtoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char RCmsgbuf[128];
  char *RCmsg = RCmsgbuf;
  YYSIZE_T RCmsg_alloc = sizeof RCmsgbuf;
#endif

  /* Three stacks and their tools:
     `RCss': related to states,
     `RCvs': related to semantic values,
     `RCls': related to locations.

     Refer to the stacks thru separate pointers, to allow RCoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  RCtype_int16 RCssa[YYINITDEPTH];
  RCtype_int16 *RCss = RCssa;
  RCtype_int16 *RCssp;

  /* The semantic value stack.  */
  YYSTYPE RCvsa[YYINITDEPTH];
  YYSTYPE *RCvs = RCvsa;
  YYSTYPE *RCvsp;



#define YYPOPSTACK(N)   (RCvsp -= (N), RCssp -= (N))

  YYSIZE_T RCstacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE RCval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int RClen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  RCstate = 0;
  RCerrstatus = 0;
  RCnerrs = 0;
  RCchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  RCssp = RCss;
  RCvsp = RCvs;

  goto RCsetstate;

/*------------------------------------------------------------.
| RCnewstate -- Push a new state, which is found in RCstate.  |
`------------------------------------------------------------*/
 RCnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  RCssp++;

 RCsetstate:
  *RCssp = RCstate;

  if (RCss + RCstacksize - 1 <= RCssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T RCsize = RCssp - RCss + 1;

#ifdef RCoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *RCvs1 = RCvs;
	RCtype_int16 *RCss1 = RCss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if RCoverflow is a macro.  */
	RCoverflow (YY_("memory exhausted"),
		    &RCss1, RCsize * sizeof (*RCssp),
		    &RCvs1, RCsize * sizeof (*RCvsp),

		    &RCstacksize);

	RCss = RCss1;
	RCvs = RCvs1;
      }
#else /* no RCoverflow */
# ifndef YYSTACK_RELOCATE
      goto RCexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= RCstacksize)
	goto RCexhaustedlab;
      RCstacksize *= 2;
      if (YYMAXDEPTH < RCstacksize)
	RCstacksize = YYMAXDEPTH;

      {
	RCtype_int16 *RCss1 = RCss;
	union RCalloc *RCptr =
	  (union RCalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (RCstacksize));
	if (! RCptr)
	  goto RCexhaustedlab;
	YYSTACK_RELOCATE (RCss);
	YYSTACK_RELOCATE (RCvs);

#  undef YYSTACK_RELOCATE
	if (RCss1 != RCssa)
	  YYSTACK_FREE (RCss1);
      }
# endif
#endif /* no RCoverflow */

      RCssp = RCss + RCsize - 1;
      RCvsp = RCvs + RCsize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) RCstacksize));

      if (RCss + RCstacksize - 1 <= RCssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", RCstate));

  goto RCbackup;

/*-----------.
| RCbackup.  |
`-----------*/
RCbackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  RCn = RCpact[RCstate];
  if (RCn == YYPACT_NINF)
    goto RCdefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (RCchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      RCchar = YYLEX;
    }

  if (RCchar <= YYEOF)
    {
      RCchar = RCtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      RCtoken = YYTRANSLATE (RCchar);
      YY_SYMBOL_PRINT ("Next token is", RCtoken, &RClval, &RClloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  RCn += RCtoken;
  if (RCn < 0 || YYLAST < RCn || RCcheck[RCn] != RCtoken)
    goto RCdefault;
  RCn = RCtable[RCn];
  if (RCn <= 0)
    {
      if (RCn == 0 || RCn == YYTABLE_NINF)
	goto RCerrlab;
      RCn = -RCn;
      goto RCreduce;
    }

  if (RCn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (RCerrstatus)
    RCerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", RCtoken, &RClval, &RClloc);

  /* Discard the shifted token unless it is eof.  */
  if (RCchar != YYEOF)
    RCchar = YYEMPTY;

  RCstate = RCn;
  *++RCvsp = RClval;

  goto RCnewstate;


/*-----------------------------------------------------------.
| RCdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
RCdefault:
  RCn = RCdefact[RCstate];
  if (RCn == 0)
    goto RCerrlab;
  goto RCreduce;


/*-----------------------------.
| RCreduce -- Do a reduction.  |
`-----------------------------*/
RCreduce:
  /* RCn is the number of a rule to reduce with.  */
  RClen = RCr2[RCn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  RCval = RCvsp[1-RClen];


  YY_REDUCE_PRINT (RCn);
  switch (RCn)
    {
        case 2:
#line 108 "RunConfigGrammar.y"
    {
;}
    break;

  case 3:
#line 111 "RunConfigGrammar.y"
    {
;}
    break;

  case 8:
#line 122 "RunConfigGrammar.y"
    {
  NetworkState network_state;
  double value = (RCvsp[(3) - (4)].expr)->eval(NULL, network_state);
  config->setParameter((RCvsp[(1) - (4)].str), value);
  free((RCvsp[(1) - (4)].str));
  delete (RCvsp[(3) - (4)].expr);
;}
    break;

  case 9:
#line 132 "RunConfigGrammar.y"
    {
  Node* node = network->getNode((RCvsp[(1) - (6)].str));
  NetworkState network_state;
  double value = (RCvsp[(5) - (6)].expr)->eval(NULL, network_state);
  if (!strcasecmp((RCvsp[(3) - (6)].str), "istate")) {
    if (backward_istate) {
      node->setIState((bool)value);
    } else {
      if (value < 0) {
	new IStateGroup(network, node);
      } else {
	new IStateGroup(network, node, (RCvsp[(5) - (6)].expr));
      }
    }
  } else if (!strcasecmp((RCvsp[(3) - (6)].str), "is_internal")) {
    node->isInternal((bool)value);
  } else if (!strcasecmp((RCvsp[(3) - (6)].str), "refstate")) {
    if (value < 0) {
      node->setReference(false);
    } else {
      node->setReferenceState((NodeState)value);
    }
  } else {
    throw BNException(std::string(RC_error_head() + "invalid node attribute: ") + (RCvsp[(3) - (6)].str) + ", valid attributes are: istate or is_internal");
  }

  free((RCvsp[(1) - (6)].str));
  free((RCvsp[(3) - (6)].str));
  delete (RCvsp[(5) - (6)].expr);
;}
    break;

  case 10:
#line 163 "RunConfigGrammar.y"
    {
  if (strcasecmp((RCvsp[(3) - (6)].str), "istate")) {
    throw BNException(std::string(RC_error_head() + "invalid node group attribute: ") + (RCvsp[(3) - (6)].str) + ", valid attribute is istate");
  }
  std::string error_msg;
  new IStateGroup(network, (RCvsp[(1) - (6)].node_list), (RCvsp[(5) - (6)].istate_expr_list), error_msg);
  if (error_msg.length() > 0) {
    throw BNException(std::string(RC_error_head() + error_msg));
  }
  
  free((RCvsp[(3) - (6)].str));
;}
    break;

  case 11:
#line 178 "RunConfigGrammar.y"
    {
  (RCval.node_list) = (RCvsp[(2) - (3)].node_list);
;}
    break;

  case 12:
#line 184 "RunConfigGrammar.y"
    {
  (RCval.node_list) = new std::vector<const Node*>();
  (RCval.node_list)->push_back(network->getNode((RCvsp[(1) - (1)].str)));
  free((RCvsp[(1) - (1)].str));
;}
    break;

  case 13:
#line 190 "RunConfigGrammar.y"
    {
  (RCval.node_list) = (RCvsp[(1) - (3)].node_list);
  (RCval.node_list)->push_back(network->getNode((RCvsp[(3) - (3)].str)));
;}
    break;

  case 14:
#line 197 "RunConfigGrammar.y"
    {
  (RCval.istate_expr_list) = new std::vector<IStateGroup::ProbaIState*>();
  (RCval.istate_expr_list)->push_back((RCvsp[(1) - (1)].istate_expr));
;}
    break;

  case 15:
#line 202 "RunConfigGrammar.y"
    {
  (RCval.istate_expr_list) = (RCvsp[(1) - (3)].istate_expr_list);
  (RCval.istate_expr_list)->push_back((RCvsp[(3) - (3)].istate_expr));
;}
    break;

  case 16:
#line 209 "RunConfigGrammar.y"
    {
  (RCval.istate_expr) = new IStateGroup::ProbaIState((RCvsp[(1) - (4)].expr), (RCvsp[(3) - (4)].expr_list));
  delete (RCvsp[(1) - (4)].expr);
  for (std::vector<Expression*>::iterator it = (RCvsp[(3) - (4)].expr_list)->begin(); it != (RCvsp[(3) - (4)].expr_list)->end(); ++it) {
    delete *it;
  }
  delete (RCvsp[(3) - (4)].expr_list);
;}
    break;

  case 17:
#line 220 "RunConfigGrammar.y"
    {
  (RCval.expr_list) = new std::vector<Expression*>();
  (RCval.expr_list)->push_back((RCvsp[(1) - (1)].expr));
;}
    break;

  case 18:
#line 225 "RunConfigGrammar.y"
    {
  (RCval.expr_list) = (RCvsp[(1) - (3)].expr_list);
  (RCval.expr_list)->push_back((RCvsp[(3) - (3)].expr));
;}
    break;

  case 19:
#line 232 "RunConfigGrammar.y"
    {
  const Symbol* symbol = network->getSymbolTable()->getOrMakeSymbol((RCvsp[(1) - (4)].str));
  free((RCvsp[(1) - (4)].str));
  NetworkState dummy_state;
  network->getSymbolTable()->setSymbolValue(symbol, (RCvsp[(3) - (4)].expr)->eval(NULL, dummy_state));
  delete (RCvsp[(3) - (4)].expr);
;}
    break;

  case 20:
#line 242 "RunConfigGrammar.y"
    {
  (RCval.expr) = new ConstantExpression((RCvsp[(1) - (1)].l));
;}
    break;

  case 21:
#line 246 "RunConfigGrammar.y"
    {
  (RCval.expr) = new ConstantExpression((RCvsp[(1) - (1)].d));
;}
    break;

  case 22:
#line 250 "RunConfigGrammar.y"
    {
  (RCval.expr) = new SymbolExpression(network->getSymbolTable(), network->getSymbolTable()->getOrMakeSymbol((RCvsp[(1) - (1)].str)));
  free((RCvsp[(1) - (1)].str));
;}
    break;

  case 23:
#line 255 "RunConfigGrammar.y"
    {
  (RCval.expr) = new ParenthesisExpression((RCvsp[(2) - (3)].expr));
;}
    break;

  case 24:
#line 261 "RunConfigGrammar.y"
    {
  (RCval.arg_list) = new ArgumentList();
  (RCval.arg_list)->push_back((RCvsp[(1) - (1)].expr));
;}
    break;

  case 25:
#line 266 "RunConfigGrammar.y"
    {
  (RCval.arg_list) = (RCvsp[(1) - (3)].arg_list);
  (RCval.arg_list)->push_back((RCvsp[(3) - (3)].expr));
;}
    break;

  case 26:
#line 274 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 27:
#line 278 "RunConfigGrammar.y"
    {
  (RCval.expr) = new FuncCallExpression((RCvsp[(1) - (4)].str), (RCvsp[(3) - (4)].arg_list));
  free((RCvsp[(1) - (4)].str));
;}
    break;

  case 28:
#line 283 "RunConfigGrammar.y"
    {
  (RCval.expr) = new FuncCallExpression((RCvsp[(1) - (3)].str), NULL);
  free((RCvsp[(1) - (3)].str));
;}
    break;

  case 29:
#line 291 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 30:
#line 295 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(2) - (2)].expr);
;}
    break;

  case 31:
#line 299 "RunConfigGrammar.y"
    {
  (RCval.expr) = new SubExpression(new ConstantExpression(0.0), (RCvsp[(2) - (2)].expr));
;}
    break;

  case 32:
#line 303 "RunConfigGrammar.y"
    {
  (RCval.expr) = new NotLogicalExpression((RCvsp[(2) - (2)].expr));
;}
    break;

  case 33:
#line 307 "RunConfigGrammar.y"
    {
  (RCval.expr) = new NotLogicalExpression((RCvsp[(2) - (2)].expr));
;}
    break;

  case 34:
#line 313 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 35:
#line 317 "RunConfigGrammar.y"
    {
  (RCval.expr) = new MulExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 36:
#line 321 "RunConfigGrammar.y"
    {
  (RCval.expr) = new DivExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 37:
#line 327 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 38:
#line 331 "RunConfigGrammar.y"
    {
  (RCval.expr) = new AddExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 39:
#line 335 "RunConfigGrammar.y"
    {
  (RCval.expr) = new SubExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 40:
#line 341 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 41:
#line 345 "RunConfigGrammar.y"
    {
  (RCval.expr) = new LetterExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 42:
#line 349 "RunConfigGrammar.y"
    {
  (RCval.expr) = new GreaterExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 43:
#line 353 "RunConfigGrammar.y"
    {
  (RCval.expr) = new LetterOrEqualExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 44:
#line 357 "RunConfigGrammar.y"
    {
  (RCval.expr) = new GreaterOrEqualExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 45:
#line 363 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 46:
#line 367 "RunConfigGrammar.y"
    {
  (RCval.expr) = new EqualExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 47:
#line 371 "RunConfigGrammar.y"
    {
  (RCval.expr) = new NotEqualExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 48:
#line 377 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 49:
#line 381 "RunConfigGrammar.y"
    {
  (RCval.expr) = new AndLogicalExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 50:
#line 387 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 51:
#line 391 "RunConfigGrammar.y"
    {
  (RCval.expr) = new OrLogicalExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 52:
#line 397 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 53:
#line 401 "RunConfigGrammar.y"
    {
  (RCval.expr) = new XorLogicalExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 54:
#line 405 "RunConfigGrammar.y"
    {
  (RCval.expr) = new XorLogicalExpression((RCvsp[(1) - (3)].expr), (RCvsp[(3) - (3)].expr));
;}
    break;

  case 55:
#line 411 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;

  case 56:
#line 415 "RunConfigGrammar.y"
    {
  (RCval.expr) = new CondExpression((RCvsp[(1) - (5)].expr), (RCvsp[(3) - (5)].expr), (RCvsp[(5) - (5)].expr));
;}
    break;

  case 57:
#line 421 "RunConfigGrammar.y"
    {
  (RCval.expr) = (RCvsp[(1) - (1)].expr);
;}
    break;


/* Line 1267 of yacc.c.  */
#line 1925 "RunConfigGrammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", RCr1[RCn], &RCval, &RCloc);

  YYPOPSTACK (RClen);
  RClen = 0;
  YY_STACK_PRINT (RCss, RCssp);

  *++RCvsp = RCval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  RCn = RCr1[RCn];

  RCstate = RCpgoto[RCn - YYNTOKENS] + *RCssp;
  if (0 <= RCstate && RCstate <= YYLAST && RCcheck[RCstate] == *RCssp)
    RCstate = RCtable[RCstate];
  else
    RCstate = RCdefgoto[RCn - YYNTOKENS];

  goto RCnewstate;


/*------------------------------------.
| RCerrlab -- here on detecting error |
`------------------------------------*/
RCerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!RCerrstatus)
    {
      ++RCnerrs;
#if ! YYERROR_VERBOSE
      RCerror (YY_("syntax error"));
#else
      {
	YYSIZE_T RCsize = RCsyntax_error (0, RCstate, RCchar);
	if (RCmsg_alloc < RCsize && RCmsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T RCalloc = 2 * RCsize;
	    if (! (RCsize <= RCalloc && RCalloc <= YYSTACK_ALLOC_MAXIMUM))
	      RCalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (RCmsg != RCmsgbuf)
	      YYSTACK_FREE (RCmsg);
	    RCmsg = (char *) YYSTACK_ALLOC (RCalloc);
	    if (RCmsg)
	      RCmsg_alloc = RCalloc;
	    else
	      {
		RCmsg = RCmsgbuf;
		RCmsg_alloc = sizeof RCmsgbuf;
	      }
	  }

	if (0 < RCsize && RCsize <= RCmsg_alloc)
	  {
	    (void) RCsyntax_error (RCmsg, RCstate, RCchar);
	    RCerror (RCmsg);
	  }
	else
	  {
	    RCerror (YY_("syntax error"));
	    if (RCsize != 0)
	      goto RCexhaustedlab;
	  }
      }
#endif
    }



  if (RCerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (RCchar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (RCchar == YYEOF)
	    YYABORT;
	}
      else
	{
	  RCdestruct ("Error: discarding",
		      RCtoken, &RClval);
	  RCchar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto RCerrlab1;


/*---------------------------------------------------.
| RCerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
RCerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label RCerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto RCerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (RClen);
  RClen = 0;
  YY_STACK_PRINT (RCss, RCssp);
  RCstate = *RCssp;
  goto RCerrlab1;


/*-------------------------------------------------------------.
| RCerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
RCerrlab1:
  RCerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      RCn = RCpact[RCstate];
      if (RCn != YYPACT_NINF)
	{
	  RCn += YYTERROR;
	  if (0 <= RCn && RCn <= YYLAST && RCcheck[RCn] == YYTERROR)
	    {
	      RCn = RCtable[RCn];
	      if (0 < RCn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (RCssp == RCss)
	YYABORT;


      RCdestruct ("Error: popping",
		  RCstos[RCstate], RCvsp);
      YYPOPSTACK (1);
      RCstate = *RCssp;
      YY_STACK_PRINT (RCss, RCssp);
    }

  if (RCn == YYFINAL)
    YYACCEPT;

  *++RCvsp = RClval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", RCstos[RCn], RCvsp, RClsp);

  RCstate = RCn;
  goto RCnewstate;


/*-------------------------------------.
| RCacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
RCacceptlab:
  RCresult = 0;
  goto RCreturn;

/*-----------------------------------.
| RCabortlab -- YYABORT comes here.  |
`-----------------------------------*/
RCabortlab:
  RCresult = 1;
  goto RCreturn;

#ifndef RCoverflow
/*-------------------------------------------------.
| RCexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
RCexhaustedlab:
  RCerror (YY_("memory exhausted"));
  RCresult = 2;
  /* Fall through.  */
#endif

RCreturn:
  if (RCchar != YYEOF && RCchar != YYEMPTY)
     RCdestruct ("Cleanup: discarding lookahead",
		 RCtoken, &RClval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (RClen);
  YY_STACK_PRINT (RCss, RCssp);
  while (RCssp != RCss)
    {
      RCdestruct ("Cleanup: popping",
		  RCstos[*RCssp], RCvsp);
      YYPOPSTACK (1);
    }
#ifndef RCoverflow
  if (RCss != RCssa)
    YYSTACK_FREE (RCss);
#endif
#if YYERROR_VERBOSE
  if (RCmsg != RCmsgbuf)
    YYSTACK_FREE (RCmsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (RCresult);
}


#line 426 "RunConfigGrammar.y"


void runconfig_setNetwork(Network* _network)
{
  network = _network;
}

void runconfig_setConfig(RunConfig* _config)
{
  config = _config;
}

#include "lex.RC.cc"

void RC_scan_expression(const char* str)
{
    RC_switch_to_buffer(RC_scan_string(str));
}
