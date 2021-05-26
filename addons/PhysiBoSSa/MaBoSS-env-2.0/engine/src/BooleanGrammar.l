
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
     BooleanGrammar.l

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#include <math.h>
#include "Utils.h"

static char *yytokstr();
static unsigned int input_lineno = 1;
static void skip_comment(void);
%}

fracconst	([0-9]+\.[0-9]+)|([0-9]+\.)|(\.[0-9]+)
exppart		[eE](\-|\+)?[0-9]+

%%

[\t\f\v\r ]+ { }
"\n" { input_lineno++; }
"/*" { skip_comment(); }
"//".* { }
[a-zA-Z_][a-zA-Z_0-9]* {
  if (!strcasecmp(yytext, "NOT")) {
     return LOGNOT;
  }
  if (!strcasecmp(yytext, "AND")) {
     return LOGAND;
  }
  if (!strcasecmp(yytext, "OR")) {
     return LOGOR;
  }
  if (!strcasecmp(yytext, "XOR")) {
     return LOGXOR;
  }
  if (!strcasecmp(yytext, "NODE")) {
     return NODE;
  }
  yylval.str = maboss_strdup(yytext);
  return IDENTIFIER;
}
"$"[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = maboss_strdup(yytext);
  return VARIABLE;
}
\" {
  yylval.str = yytokstr();
  return STRING;
}
"=="			{ return EQUAL; }
"!="			{ return NOT_EQUAL; }
"<="			{ return LTEQ; }
">="			{ return GTEQ; }
"&&"|"&"		{ return LOGAND; }
"||"|"|"		{ return LOGOR; }
"!"			{ return LOGNOT; }
[0-9]+			{ sscanf(yytext, "%lld", &yylval.l); return INTEGER; }
{fracconst}{exppart}?	{ yylval.d = atof(yytext); return DOUBLE; }
[0-9]+{exppart}		{ yylval.d = atof(yytext); return DOUBLE; }
.			{ return yytext[0]; }

%%

#define _ESC_(X, Y) case X: *p++ = Y; break

static void yypurgestring(unsigned char *s)
{
  unsigned char c;
  unsigned char *p = s;
  while ((c = *s) != 0) {
    if (c == '\\') {
       c = *++s;
       switch(c) {
         _ESC_('a', '\a');
	 _ESC_('b', '\b');
	 _ESC_('f', '\f');
	 _ESC_('n', '\n');
	 _ESC_('r', '\r');
	 _ESC_('t', '\t');
	 _ESC_('v', '\v');
	 _ESC_('\'', '\'');
	 _ESC_('\"', '"');
	 _ESC_('\\', '\\');

	 default:
	   *p++ = '\\';
	   *p++ = c;
        }
       s++;
      }
      else
        *p++ = *s++;
    }

  *p = 0;
}

static char *yytokstr()
{
   int s_size;
   unsigned char *s, c;
   s_size = 32;
   s = (unsigned char *)malloc(s_size);

   int n, backslash;
   for (n = 0, backslash = 0; (c = yyinput()) != 0 && c != 255; n++) {
       if (c == '\n') {
         input_lineno++;
       }
       else if (c == '\\') {
         backslash = !backslash;
       }
       else if (c == '"' && !backslash) {
	 break;
       }
       else {
         backslash = 0;
       }

       if (n >= s_size) {
           s_size *= 2;
           s = (unsigned char *)realloc(s, s_size);
       }

       s[n] = c;
     }

   if (n >= s_size) {
       s_size += 4;
       s = (unsigned char *)realloc(s, s_size);
   }
   
   s[n] = 0;
   yypurgestring(s);
   return (char *)s;
}

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

static void yyerror(const char *)
{
  char tok[32];
  sprintf(tok, "%u", input_lineno);
  throw BNException("BND syntax error at line #" + std::string(tok));
}

void yy_scan_expression(const char* str)
{
    yy_switch_to_buffer(yy_scan_string(str));
}

// to avoid compiler warning when using -Wall option
void boolean_grammar_to_make_compiler_happy()
{
  unput('x');
}
