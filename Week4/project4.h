void yyerror(char *s);

struct symbol {
	char *name;
	double value;

	// v as value, a as array
	char type;
	int arr_len;
	int ini_index;
	double *arr_head;
};

#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

struct symlist {
	struct symbol *sym;
	struct symlist *next;
};

struct numlist {
	double n;
	struct numlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
struct numlist *newnumlist(double n, struct numlist *nl);

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct printcall {
	int nodetype;
	struct ast *l;
};

struct flow {
	int nodetype;
	struct ast *cond;
	struct ast *tl;
	struct ast *el;
};

struct numval {
	int nodetype;
	double number;
};

struct symref {
	int nodetype;
	struct symbol *s;
};

struct symrefarr {
	// nodetype: U
	int nodetype;
	struct symbol *s;
	struct ast *index;
};

struct symasgn {
	int nodetype;
	struct symbol *s;
	struct ast *v;
};

struct symasgnarr {
	// nodetype: V
	int nodetype;
	struct symbol *s;
	struct ast *index;
	struct ast *v;
};

struct syminitarr {
	// nodetype: T
	int nodetype;
	struct symbol *s;
	struct numlist *nl;
};

struct decl {
	int nodetype;
	struct symlist *sl;
	char type;
};

struct declarr{
	int nodetype;
	struct symlist *sl;
	int len;
	int shift;
	char type;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newprint(struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

struct ast *newdecl(struct symlist *sl, char type);
struct ast *newdeclarr(struct symlist *sl, int begin, int end, char type);
struct ast *newasgnarr(struct symbol *s, struct ast *index, struct ast *a);
struct ast *newrefarr(struct symbol *s, struct ast *index);
struct ast *newinitarr(struct symbol *s, struct numlist *nl);

double eval(struct ast *);

void treefree(struct ast *);
