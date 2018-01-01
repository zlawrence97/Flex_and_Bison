# Flex and Bison - GWU CSCI 3313 Fall 2017 : **Language parser compiling project**
_Be sure to change the file names to correspond to the current week._


Bison: .y compiled: tab.c and tab.h

Flex: .l compiled: lex.yy.c

C: .c compiled: .o

Header: .h


_**Week 1 Assignment : Advanced Calculator Lexer**_

Refer to page 68 of [Flex & Bison] (http://web.iitd.ac.in/~sumeet/flex__bison.pdf) to fill in the missing lines of code
provided by the syllabus in `project1.l` . Compile the completed with `flex project1.l` which will create a `lex.yy.c` file.

**Submit**: project1.l

_**Week 2 Assignment : Compile base code**_

Use template header file, BE SURE TO CHANGE NAMES OF HEADER FILES. Copy given Bison grammar into `project2.h`, which will be used for the remainder of the project. Compile .y file using `bison -d project2.y`.

**Submit**: project2.l, lex.yy.c, project2.y, project2.tab.c & project2.tab.h, project2.h

_**Week 3 Assignement : Bubble Sort**_

Finish the implementation of bubble sort in `sort.txt` file and submit.

**Submit**: sort.txt

_**Week 4 Assignment**_

Use project1.l, the Header file from the template files, project2.y, and the sort.txt developed during week 3.
Add the functions `static unsigned synhash(char *sym)` and `struct symbol * lookup(char *sym)` to your `project4.c` file. Compile using the following commands:
```
$flex project4.l
$bison -d project4.y
$gcc -o project4 project4.c project4.tab.c lex.yy.c
$./project4 sort.txt
```
**Submit**: project4.l, lex.yy.c, project4.y, project4.tab.c and project4.tab.h, project4.h, sort.txt and project4.c

_**Week 5 Assignment**_

Add the following functions to `project5.c` given in syllabus:

```
struct ast * newast(int nodetype, struct ast *l, struct ast *r)
struct ast * newnum(double d)
struct ast * newprint(struct ast *l)
struct ast * newref(struct symbol *s)
struct ast * newdecl(struct symlist *sl, char type)
struct ast * newdeclarr(struct symlist *sl, int begin, int end, char type)
void treefree(struct ast *a)
struct symlist * newsymlist(struct symbol *sym, struct symlist *next)
double eval(struct ast *a)
```
Complete corresponding calls in `project5.y`, given in syllabus.

Compile commands:
```
$flex project5.l
$bison -d project5.y
$gcc -o project5 project5.c project5.tab.c lex.yy.c
$./project5 sort.txt
```
**Submit**: project5.l, lex.yy.c, project5.y, project5.tab.c and project5.tab.h, project5.h, sort.txt and project5.c

_**Week 6 Assignment**_

Take the functions given in the syllabus: `struct ast * newcmp(int cmptype, struct ast *l, struct ast *r)` and `struct ast * newasgn(struct symbol *s, struct ast *v)` and add them to `project6.c`.

Complete corresponding calls in `project6.y`, given in syllabus.


Compile commands:
```
$flex project6.l
$bison -d project.y
$gcc -o project6 project6.c project6.tab.c lex.yy.c
$./project6 sort.txt
```
**Submit**: project6.l, lex.yy.c, project6.y, project6.tab.c and project6.tab.h, project6.h, sort.txt and project6.c

_**Week 7 Assignment**_

Add the following functions to `project7.c`, given in syllabus:
```
struct ast * newinitarr(struct symbol *s, struct numlist *nl)
struct numlist * newnumlist(double num, struct numlist *next)
static double calldecl(struct decl *d)
static double calldeclarr(struct declarr * da)
static double callprint(struct printcall *f)
static double callinitarr(struct syminitarr *ia)
```
As well as three additional calls to `eval()` function.

Complete corresponding calls in `project7.y`, given in syllabus.

Compile commands:
```
$flex project7.l
$bison -d project7.y
$gcc -o project5 project7.c project7.tab.c lex.yy.c
$./project7 sort.txt
```
**Submit**: project7.l, lex.yy.c, project7.y, project7.tab.c and project7.tab.h, project7.h, sort.txt and project7.c

_**Week 8 Assignment**_

Add the following functions to `project8.c`, given in syllabus.
```
struct ast * newrefarr(struct symbol *s, struct ast *index)
struct ast * newasgnarr(struct symbol *s, struct ast *index, struct ast *v)
static double callrefarr(struct symrefarr *sa)
static double callasgnarr(struct symasgnarr *aa)
```
Complete corresponding calls in `project8.y`, given in syllabus.

Compile commands:
```
$flex project8.l
$bison -d project8.y
$gcc -o project8 project8.c project8.tab.c lex.yy.c
$./project8 sort.txt
```
_**Week 9 Assingment**_

Add the following funciton to `project9.c`: _struct ast * newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)_


Compile commands and result:
```
$flex project9.l
$bison -d project9.y
$gcc -o project9 project9.c project9.tab.c lex.yy.c
$./project9 sort.txt
arr = {   1,    3,    7,    5,    4}
arr = {   1,    3,    4,    5,    7}
parse done.
$
```

