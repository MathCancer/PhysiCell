
%{
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
*/

#include <math.h>
#include "Utils.h"

//static char *yytokstr();
static unsigned int input_lineno = 1;
static const char* file;
static const char* expr;
static void skip_comment(void);
%}

fracconst	([0-9]+\.[0-9]+)|([0-9]+\.)|(\.[0-9]+)
exppart		[eE](\-|\+)?[0-9]+

%%

[\t\f\v\r ]+ { }
"\n" { input_lineno++; }
"/*" { skip_comment(); }
"//".* { }
[Tt][Rr][Uu][Ee]        { yylval.l = 1; return INTEGER; }
[Ff][Aa][Ll][Ss][Ee]    { yylval.l = 0; return INTEGER; }
"$"[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = maboss_strdup(yytext);
  return VARIABLE;
}
[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = maboss_strdup(yytext);
  return SYMBOL;
}
[0-9]+			{ sscanf(yytext, "%lld", &yylval.l); return INTEGER; }
{fracconst}{exppart}?	{ yylval.d = atof(yytext); return DOUBLE; }
[0-9]+{exppart}		{ yylval.d = atof(yytext); return DOUBLE; }
0x([0-9a-fA-F]*\.[0-9a-fA-F]+|[0-9a-fA-F]+\.?)p[-+]?[0-9]+ { yylval.d = atof(yytext); return DOUBLE; }
.			{ return yytext[0]; }

%%

static void skip_comment(void)
{
  int c1, c2;

  c1 = yyinput();
  c2 = yyinput();

  while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
    if (c1 == '\n') {
      ++input_lineno;
    }
    c1 = c2;
    c2 = yyinput();
  }
}

int yywrap(void)
{
  return 1;
}

void yy_set_file(const char* _file)
{
  input_lineno = 1;
  file = _file;
  expr = NULL;
}

void yy_set_expr(const char* _expr)
{
  input_lineno = 1;
  file = NULL;
  expr = _expr;
}

static void yyerror(const char *)
{
  char tok[32];
  sprintf(tok, "%u", input_lineno);
  if (file) {
    throw BNException("configuration syntax error at line #" + std::string(tok) + " in file \"" + file + "\"");
  } else {
    throw BNException("configuration syntax error in expression '" + std::string(expr) + "'");
  }
}

std::string yy_error_head()
 {
   std::ostringstream ostr;
   ostr << "configuration parsing at line #" << input_lineno << ": ";
   return ostr.str();
 }

// to avoid compiler warning when using -Wall option
void runconfig_to_make_compiler_happy()
{
  unput('x');
}
