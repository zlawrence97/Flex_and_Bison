%{
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
#include "project5.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;
	struct symlist *sl;
	struct numlist *nl;
	int fn;
	char type_c;
}

%token <d> NUMBER
%token <s> ID
%token <prog> PROGRAM
%token <var> VAR
%token <array> ARRAY
%token <of> OF
%token <i> INTEGER
%token <real> REAL
%token <begin> BGN
%token <end> END
%token <f> IF
%token <t> THEN
%token <e> ELSE
%token <w> WHILE
%token <dd> DO
%token <dt> DOTS
%token <pr> PRINT
%token <type_c> STD_TYPE


%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <p> program
%type <a> decl_list
%type <d> decl
%type <st> stmt_list
%type <s> stmt
%type <e> exp
%type <sl> id_list
%type <nl> num_list


%start program
%%
 
program: PROGRAM ID '(' id_list ')' ';' decl_list BGN stmt_list END '.'
	{ eval($7); eval($9); treefree($9); treefree($7); printf("parse done.\n"); }
	;

decl_list:			{ /*$$ = NULL;*/ }
	| decl ';' decl_list	{if($3 == NULL) $$ = $1; else $$ = newast('L', $1, $3); }
	;

decl: VAR id_list ':' STD_TYPE						{ $$ = newdecl($2, $4); }
	| VAR id_list ':' ARRAY '[' NUMBER DOTS NUMBER ']' OF STD_TYPE	{ $$ = newdeclarr($2, $6, $8, $11); }
	;

stmt: IF exp THEN '{' stmt_list '}'				{ }
	| IF exp THEN '{' stmt_list '}' ELSE '{' stmt_list '}'	{ }
	| WHILE exp DO '{' stmt_list '}'				{ }
	| exp
	;

stmt_list: stmt				{ $$ = $1; }
	| stmt_list ';' stmt 		{ $$ = newast('L', $1, $3); }
	;

exp: exp CMP exp				{ }
	| exp '+' exp			{ $$ = newast('+', $1, $3);   }
	| exp '-' exp			{ $$ = newast('-', $1, $3);   }
	| exp '*' exp			{ $$ = newast('*', $1, $3);   }
	| exp '/' exp 			{ $$ = newast('/', $1, $3);   }
	| '|' exp			{ $$ = newast('|', $2, NULL); }
	| '(' exp ')'			{ $$ = $2;}
	| '-' exp %prec UMINUS		{ $$ = newast('M', $2, NULL); }
	| NUMBER				{ $$ = newnum($1); }
	| ID				{ $$ = newref($1); }
	| ID '[' exp ']'			{ }
	| ID '[' exp ']' '=' exp		{ }
	| ID '=' exp			{ }
	| ID '=' '{' num_list '}'	{ }
	| PRINT '(' exp ')'		{ $$ = newprint($3); }
	;

num_list: NUMBER				{ }
	| NUMBER ',' num_list		{ }
	;

id_list: ID				{ $$ = newsymlist($1, NULL); }
	| ID ',' id_list			{ $$ = newsymlist($1, $3); }
	;
%%
