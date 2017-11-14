# Flex and Bison - GWU CSCI 3313 Fall 2017
**Language parser compiling project**
_Be sure to change the file names to correspond to the current week._


Bison: .y compiled: tab.c and tab.h
Flex: .l compiled: lex.yy.c
C: .c compiled: .o
Header: .h


_Week 1 Assignment : Advanced Calculator Lexer_

Refer to page 68 of [Flex & Bison] (http://web.iitd.ac.in/~sumeet/flex__bison.pdf) to fill in the missing lines of code
provided by the syllabus in `project1.l` . Compile the completed with `flex project1.l` which will create a `lex.yy.c` file.

**Submit**: project1.l

_Week 2 Assignment : Compile base code_

Use template header file, BE SURE TO CHANGE NAMES OF HEADER FILES. Copy given Bison grammar into `project2.h`, which will be used for the remainder of the project. Compile .y file using `bison -d project2.y`.

**Submit**: project2.l, lex.yy.c, project2.y, project2.tab.c & project2.tab.h, project2.h

_Week 3 Assignement : Bubble Sort_

Finish the implementation of bubble sort in `sort.txt` file and submit.

**Submit**: sort.txt

_Week 4 Assignment : _

Use project1.l, the Header file from the template files, project2.y, and the sort.txt developed during week 3.
Add the functions `static unsigned synhash(char *sym)` and `struct symbol * lookup(char *sym)` to your `project4.c` file. Compile using the following commands:
$`flex project4.l`
$`bison -d project4.y`
$`gcc -o project4 project4.c project4.tab.c lex.yy.c`
$`./project4 sort.txt` _parses text file_

**Submit**: project4.l, lex.yy.c, project4.y, project4.tab.c and project4.tab.h, project4.h, sort.txt and project4.c

_**Updates for weekly submissions will follow**_
