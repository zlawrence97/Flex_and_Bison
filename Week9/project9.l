/* recognize tokens for the calculator */

%option noyywrap nodefault yylineno
%{
#include "project7.h"
#include "project7.tab.h"
%}


EXP ([Ex][-+]?[0-9]+)

%%



".."	{ return DOTS; }

"+"	|
"-"     |
"*"     |
"/"     |
"="     |
"|"     |
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


">"	{ yylval.fn = 1; return CMP; }
"<"     { yylval.fn = 2; return CMP; }
"<>"    { yylval.fn = 3; return CMP; }
"=="    { yylval.fn = 4; return CMP; }
">="    { yylval.fn = 5; return CMP; }
"<="	{ yylval.fn = 6; return CMP; }

"integer"	{ yylval.type_c = 'a'; return STD_TYPE; }
"real"		{ yylval.type_c = 'b'; return STD_TYPE; }

"program"	{ return PROGRAM; }
"var"		{ return VAR; }
"array"		{ return ARRAY; }
"of"		{ return OF; }
"begin"		{ return BGN; }
"end"		{ return END; }



"if"	{ return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"	{ return DO; }

"print"	{ return PRINT; }



[a-zA-Z][a-zA-Z0-9]*	{ yylval.s = lookup(yytext); return ID; }

[0-9]+"."[0-9]+	|
[0-9]+	{ yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t\n]
.	{ yyerror("Mystery character.\n"); }
%%
