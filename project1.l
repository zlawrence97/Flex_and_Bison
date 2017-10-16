/* recognize tokens for the calculator */

%option noyywrap nodefault yylineno
%{
#include "project1.h"
#include "project1.tab.h"
%}

/* float exponent */
EXP ([Ex][-+]?[0-9]+)

%%

/* single character ops */

".."	{ return DOTS; }

"+"	|
…
…
…
…
…
","	|
";"	|
":"	|
"."	|
"["	|
"]"	|
"{"	|
"}"	|
"("	|
")"	{ return yytext[0]; }

/* comparison ops, all are a CMP token */
">"	{ yylval.fn = 1; return CMP; }
…
…
…
…
"<="	{ yylval.fn = 6; return CMP; }

"integer"	{ yylval.type_c = 'a'; return STD_TYPE; }
"real"		{ yylval.type_c = 'b'; return STD_TYPE; }

"program"	{ return PROGRAM; }
"var"		{ return VAR; }
"array"		{ return ARRAY; }
"of"		{ return OF; }
"begin"		{ return BGN; }
"end"		{ return END; }

/* keywords */

"if"	{ return IF; }
…
…
…
"do"	{ return DO; }

"print"	{ return PRINT; }

/* names */

[a-zA-Z][a-zA-Z0-9]*	{ yylval.s = lookup(yytext); return ID; }

[0-9]+"."[0-9]+	|
[0-9]+	{ yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t\n]
.	{ yyerror("Mystery character.\n"); }
%%
