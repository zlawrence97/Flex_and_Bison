%{
#include <stdio.h>
#include <stdlib.h>
#include "project4.h"
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
%token <p> PROGRAM
%token <v> VAR
%token <a> ARRAY
%token <of> OF
%token <i> INTEGER
%token <r> REAL
%token <b> BGN
%token <e> END
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
	{ printf("new program.\n"); }
	;

decl_list:	{ /*$$ = NULL;*/ }
	| decl ';' decl_list	{ printf("new declaration.\n"); }
	;

decl: VAR id_list ':' STD_TYPE	{ }
	| VAR id_list ':' ARRAY '[' NUMBER DOTS NUMBER ']' OF STD_TYPE
	{ }
	;

stmt: IF exp THEN '{' stmt_list '}'		{ }
	| IF exp THEN '{' stmt_list '}' ELSE '{' stmt_list '}'	{ }
	| WHILE exp DO '{' stmt_list '}'	{ }
	| exp
	;

stmt_list: stmt		{  printf("new statement.\n"); }
	| stmt_list ';' stmt { }
	;

exp: exp CMP exp	{ }
	| exp '+' exp	{ }
	| exp '-' exp	{ }
	| exp '*' exp	{ }
	| exp '/' exp 	{ }
	| '|' exp	{ }
	| '(' exp ')'	{ }
	| '-' exp %prec UMINUS	{ }
	| NUMBER	{ }
	| ID		{ }
	| ID '[' exp ']'	{ }
	| ID '[' exp ']' '=' exp	{ }
	| ID '=' exp	{ }
	| ID '=' '{' num_list '}'	{ }	
	| PRINT '(' exp ')'	{ }
	;

num_list: NUMBER	{ }	
	| NUMBER ',' num_list	{ }
	;

id_list: ID	{ }
	| ID ',' id_list	{ }
	;
%%
