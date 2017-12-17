#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project9.h"

int yyparse(void);

static unsigned symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while(c == *sym++) hash = hash*9 ^ c;

	return hash;
}

struct symbol * lookup(char *sym)
{
	struct symbol *sp = &symtab[symhash(sym) % NHASH];
	int scount = NHASH;

	while(--scount >= 0) {
		if(sp->name && !strcmp(sp->name, sym)) { return sp; }

		if(!sp->name) {
			sp->name = strdup(sym);
			sp->value = 0;
			return sp;
		}

		if(++sp >= symtab + NHASH) sp = symtab;
	}
	yyerror("symbol table overflow\n");
	abort();
}

struct ast * newast(int nodetype, struct ast *l, struct ast *r)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast * newnum(double d)
{
	struct numval *a = malloc(sizeof(struct numval));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'K';
	a->number = d;
	return (struct ast *)a;
}

struct ast * newcmp(int cmptype, struct ast *l, struct ast *r)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = '0' + cmptype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast * newprint(struct ast *l)
{
	struct printcall *a = malloc(sizeof(struct printcall));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'P';
	a->l = l;
	return (struct ast *)a;
}

struct ast * newref(struct symbol *s)
{
	struct symref *a = malloc(sizeof(struct symref));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'N';
	a->s = s;
	return (struct ast *)a;
}

struct ast * newrefarr(struct symbol *s, struct ast *index)
{
	struct symrefarr *a = malloc(sizeof(struct symrefarr));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'U';
	a->s = s;
	a->index = index;
	return (struct ast *)a;
}

struct ast * newasgnarr(struct symbol *s, struct ast *index, struct ast *v)
{
	struct symasgnarr *a = malloc(sizeof(struct symasgnarr));
	
	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'V';
	a->s = s;
	a->index = index;
	a->v = v;
	return (struct ast *)a;
}	

struct ast * newasgn(struct symbol *s, struct ast *v)
{
	struct symasgn *a = malloc(sizeof(struct symasgn));
	
	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = '=';
	a->s = s;
	a->v = v;
	return (struct ast *)a;
}

struct ast * newinitarr(struct symbol *s, struct numlist *nl)
{
	struct syminitarr *a = malloc(sizeof(struct syminitarr));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'T';
	a->s = s;
	a->nl = nl;
	return (struct ast *)a;
}

struct ast * newdecl(struct symlist *sl, char type)
{
	struct decl *a = malloc(sizeof(struct decl));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'X';
	a->sl = sl;
	a->type = type;
	return (struct ast *)a;
}

struct ast * newdeclarr(struct symlist *sl, int begin, int end, char type)
{
	struct declarr *a = malloc(sizeof(struct declarr));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'Y';
	a->sl = sl;
	a->len = end - begin + 1;
	if(a->len < 1) {
		yyerror("too small size for array");
		exit(0);
	}
	a->shift = begin;
	a->type = type;
	return (struct ast *)a;
}

struct ast * newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
	struct flow *a = malloc(sizeof(struct flow));

	if(!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = nodetype;
	a->cond = cond;
	a->tl = tl;
	a->el = el;
	return (struct ast *)a;
}

void treefree(struct ast *a)
{
	switch(a->nodetype) {
	
	case '+':
	case '-':
	case '*':
	case '/':
	case '1': case '2': case '3': case '4': case '5': case '6':
	case 'L':
		treefree(a->r);

	case '|':
	case 'M': case 'P':
		treefree(a->l);
	
	case 'K': case 'N': case 'U': case 'V': case 'T':
		break;

	case '=':
		free(((struct symasgn *)a)->v);
		break;

	case 'I': case 'W':
		free(((struct flow *)a)->cond);
		if(((struct flow *)a)->tl) treefree(((struct flow *)a)->tl);
		if(((struct flow *)a)->el) treefree(((struct flow *)a)->el);
		break;
	
	case 'X': 
		free(((struct decl *)a)->sl);
		break;

	case 'Y':
		free(((struct declarr *)a)->sl);
		break;

	default: printf("internal error: free bad node %c\n", a->nodetype);
	}
	
	free(a);
}

struct symlist * newsymlist(struct symbol *sym, struct symlist *next)
{
	struct symlist *sl = malloc(sizeof(struct symlist));

	if(!sl) {
		yyerror("out of space");
		exit(0);
	}
	sl->sym = sym;
	sl->next = next;
	return sl;
}

struct numlist * newnumlist(double num, struct numlist *next)
{
	struct numlist *nl = malloc(sizeof(struct numlist));

	if(!nl) {
		yyerror("out of space");
		exit(0);
	}
	nl->n = num;
	nl->next = next;
	return nl;
}

static double callprint(struct printcall *);
static double calldecl(struct decl *);
static double calldeclarr(struct declarr *);
static double callrefarr(struct symrefarr *);
static double callasgnarr(struct symasgnarr *);
static double callinitarr(struct syminitarr *);

double eval(struct ast *a)
{
	double v;
	
	if(!a) {
		yyerror("internal error, null eval");
		return 0.0;
	}

	switch(a->nodetype) {
	
	case 'K': v = ((struct numval *)a)->number; break;

	case 'N': if((((struct symref *)a)->s)->type != 'a' && (((struct symref *)a)->s)->type != 'b') 
		{ printf("using undeclared ID: %s\n", (((struct symref *)a)->s)->name); } 
		v = (((struct symref *)a)->s)->value; break;

	case '=': if((((struct symasgn *)a)->s)->type != 'a' && (((struct symasgn *)a)->s)->type != 'b')
		{ printf("using undeclared ID: %s\n", (((struct symasgn *)a)->s)->name); } 
		v = ((struct symasgn *)a)->s->value = 
		eval(((struct symasgn *)a)->v); break;

	case '+': v = eval(a->l) + eval(a->r); break;
	case '-': v = eval(a->l) - eval(a->r); break;
	case '*': v = eval(a->l) * eval(a->r); break;
	case '/': v = eval(a->l) / eval(a->r); break;
	case '|': v = fabs(eval(a->l)); break;
	case 'M': v = -eval(a->l); break;

	case '1': v = (eval(a->l) > eval(a->r))? 1 : 0; break;
	case '2': v = (eval(a->l) < eval(a->r))? 1 : 0; break;
	case '3': v = (eval(a->l) != eval(a->r))? 1 : 0; break;
	case '4': v = (eval(a->l) == eval(a->r))? 1 : 0; break;
	case '5': v = (eval(a->l) >= eval(a->r))? 1 : 0; break;
	case '6': v = (eval(a->l) <= eval(a->r))? 1 : 0; break;

	case 'I':
		if(eval(((struct flow *)a)->cond) != 0) {
			if(((struct flow *)a)->tl) {
				v = eval(((struct flow *)a)->tl);
			} else
				v = 0.0;
		} else {
			if(((struct flow *)a)->el) {
				v = eval(((struct flow *)a)->el);
			} else
				v = 0.0;
		}
		break;

	case 'W':
		v = 0.0;

		if(((struct flow *)a)->tl) {
			while(eval(((struct flow *)a)->cond) != 0)
				v = eval(((struct flow *)a)->tl);
		}
		break;

	case 'L': eval(a->l); v = eval(a->r); break;

	case 'P': v = callprint((struct printcall *)a); break;

	case 'X': v = calldecl((struct decl *)a); break;

	case 'Y': v = calldeclarr((struct declarr *)a); break;

	case 'U': if((((struct symrefarr *)a)->s)->type != 'a' && (((struct symrefarr *)a)->s)->type != 'b')
		{ printf("using undeclared ID: %s\n", (((struct symrefarr *)a)->s)->name); }
		v = callrefarr((struct symrefarr *)a); break;

	case 'V': if((((struct symasgnarr *)a)->s)->type != 'a' && (((struct symasgnarr *)a)->s)->type != 'b')
		{ printf("using undeclared ID: %s\n", (((struct symasgnarr *)a)->s)->name); }
		v = callasgnarr((struct symasgnarr *)a); break;

	case 'T': if((((struct syminitarr *)a)->s)->type != 'a' && (((struct syminitarr *)a)->s)->type != 'b')
		{ printf("using undeclared ID: %s\n", (((struct syminitarr *)a)->s)->name); }
		v = callinitarr((struct syminitarr *)a); break;

	default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

static double calldecl(struct decl *d)
{
	if(d->type == 'a') {
		while(d->sl) {
			((d->sl)->sym)->value = 0;
			((d->sl)->sym)->type = 'a';
			((d->sl)->sym)->arr_len = 0;
			((d->sl)->sym)->ini_index = 0;
			((d->sl)->sym)->arr_head = NULL;
			//printf("symbol: %s, type: %c\n", ((d->sl)->sym)->name, ((d->sl)->sym)->type);
			d->sl = (d->sl)->next;
		}
	} else if(d->type == 'b') {
		while(d->sl) {
			((d->sl)->sym)->value = 0.0;
			((d->sl)->sym)->type = 'b';
			((d->sl)->sym)->arr_len = 0;
			((d->sl)->sym)->ini_index = 0;
			((d->sl)->sym)->arr_head = NULL;
			//printf("symbol: %s, type: %c\n", ((d->sl)->sym)->name, ((d->sl)->sym)->type);
			d->sl = (d->sl)->next;
		}
	} else {
		printf("bad declaration type: %c\n", d->type);
		return 1;	
	}
	return 0;
}

static double calldeclarr(struct declarr *da)
{
	if(da->type == 'a') {
		while(da->sl) {
			((da->sl)->sym)->value = -1;
			((da->sl)->sym)->type = 'a';
			((da->sl)->sym)->arr_len = da->len;
			((da->sl)->sym)->ini_index = da->shift;
			((da->sl)->sym)->arr_head = (double *)malloc(sizeof(double) * da->len);
			if(!(((da->sl)->sym)->arr_head)) {
				yyerror("out of space for array");
				exit(0);
			}
			//printf("symbol: %s, type: %c\n", ((da->sl)->sym)->name, ((da->sl)->sym)->type);
			da->sl = (da->sl)->next;
		}	
	} else if(da->type == 'b') {
		while(da->sl) {
			((da->sl)->sym)->value = -1.0;
			((da->sl)->sym)->type = 'b';
			((da->sl)->sym)->arr_len = da->len;
			((da->sl)->sym)->ini_index = da->shift;
			((da->sl)->sym)->arr_head = (double *)malloc(sizeof(double) * da->len);
			if(!(((da->sl)->sym)->arr_head)) {
				yyerror("out of space for array");
				exit(0);
			}
			//printf("symbol: %s, type: %c\n", ((da->sl)->sym)->name, ((da->sl)->sym)->type);
			da->sl = (da->sl)->next;
		}
	} else {
		printf("bad declaration type %c\n", da->type);
		return 1;
	}
	return 0;
}

static double callprint(struct printcall *f)
{
	int i;
	if((f->l)->nodetype == 'N') {
		printf("%s", (((struct symref *)(f->l))->s)->name);
	}
	if((f->l)->nodetype == 'N' && (((struct symref *)(f->l))->s)->arr_len > 0) {
		printf(" = {");
		for(i = 0; i < (((struct symref *)(f->l))->s)->arr_len - 1; i++) {
			printf("%4.4g, ", *((((struct symref *)(f->l))->s)->arr_head + i));
		}
		printf("%4.4g", *((((struct symref *)(f->l))->s)->arr_head + (((struct symref *)(f->l))->s)->arr_len - 1));
		printf("}\n");
		return 0;
	}

	double v = eval(f->l);
	printf(" = %4.4g\n", v);
	return v;
}

static double callrefarr(struct symrefarr *sa)
{
	int arr_index = (int)eval(sa->index);
	if((sa->s)->arr_len == 0) {
		yyerror("wrong reference");
		exit(0);
	}
	return *(((sa->s)->arr_head) + arr_index - ((sa->s)->ini_index));
}

static double callasgnarr(struct symasgnarr *aa)
{
	int arr_index = (int)eval(aa->index);
	double d = eval(aa->v);
	*(((aa->s)->arr_head) + arr_index - ((aa->s)->ini_index)) = d;
	return d;
}

static double callinitarr(struct syminitarr *ia)
{
	int i;
	for(i = 0; i < ((ia->s)->arr_len) && (ia->nl) != NULL; i++)
	{
		*((ia->s)->arr_head + i) = (ia->nl)->n;
		ia->nl = (ia->nl)->next;
	}
	return 0;
}

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char **argv)
{
	extern FILE *yyin;
	++argv; --argc;
	yyin = fopen(argv[0], "r");
	return yyparse();
}
