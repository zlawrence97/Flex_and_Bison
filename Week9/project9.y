%{
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
#include "project9.h"
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
%token PROGRAM VAR ARRAY OF INTEGER REAL BGN END IF THEN ELSE WHILE DO DOTS PRINT
%token <type_c> STD_TYPE

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> decl_list decl stmt_list stmt exp
%type <sl> id_list
%type <nl> num_list

%start program

%%

program: PROGRAM ID '(' id_list ')' ';' decl_list BGN stmt_list END '.'	{ eval($7); eval($9); treefree($9); treefree($7); printf("parse done.\n"); }
	;

decl_list:					{ $$ = NULL; }
	| decl ';' decl_list	{ if($3 == NULL) $$ = $1; else $$ = newast('L', $1, $3); }
	;

decl: VAR id_list ':' STD_TYPE										{ $$ = newdecl($2, $4); }
	| VAR id_list ':' ARRAY '[' NUMBER DOTS NUMBER ']' OF STD_TYPE	{ $$ = newdeclarr($2, $6, $8, $11); }
	;

stmt: IF exp THEN '{' stmt_list '}'							{ $$ = newflow('I', $2, $5, NULL); }
	| IF exp THEN '{' stmt_list '}' ELSE '{' stmt_list '}'	{ $$ = newflow('I', $2, $5, $9); }
	| WHILE exp DO '{' stmt_list '}'						{ $$ = newflow('W', $2, $5, NULL); }
	| exp
	;

stmt_list: stmt						{ $$ = $1; }
	| stmt_list ';' stmt 			{ $$ = newast('L', $1, $3); }
	;

exp: exp CMP exp					{ $$ = newcmp($2, $1, $3); }
	| exp '+' exp					{ $$ = newast('+', $1, $3); }
	| exp '-' exp					{ $$ = newast('-', $1, $3); }
	| exp '*' exp					{ $$ = newast('*', $1, $3); }
	| exp '/' exp 					{ $$ = newast('/', $1, $3); }
	| '|' exp						{ $$ = newast('|', $2, NULL); }
	| '(' exp ')'					{ $$ = $2; }
	| '-' exp %prec UMINUS			{ $$ = newast('M', $2, NULL); }
	| NUMBER						{ $$ = newnum($1); }
	| ID							{ $$ = newref($1); }
	| ID '[' exp ']'				{ $$ = newrefarr($1, $3); }
	| ID '[' exp ']' '=' exp		{ $$ = newasgnarr($1, $3, $6); }
	| ID '=' exp					{ $$ = newasgn($1, $3); }
	| ID '=' '{' num_list '}'		{ $$ = newinitarr($1, $4); }	
	| PRINT '(' exp ')'				{ $$ = newprint($3); }
	;

num_list: NUMBER					{ $$ = newnumlist($1, NULL); }	
	| NUMBER ',' num_list			{ $$ = newnumlist($1, $3); }
	;

id_list: ID							{ $$ = newsymlist($1, NULL); }
	| ID ',' id_list				{ $$ = newsymlist($1, $3); }
	;
%%
