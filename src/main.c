#include <stdlib.h>
#include <stdio.h>
#include "syntactic_analyzer.c"

extern FILE * yyin;

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;
	yyparse();
}