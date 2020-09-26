#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntactic_analyzer.c"
#include "util/verbose.h"
#include <stdbool.h>

extern FILE * yyin;

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc == 1) {
		yyin = fopen(argv[0],"r");
	}
	else if (argc == 2)
	{
		if (strcmp(argv[0], "-v") == 0) {
			setVerbose(true);
			yyin = fopen(argv[1],"r");
		}
		else{
			printf("tdsc> '%s' is not a valid argument.", argv[0]);
			exit(0);
		}
	}
	else{
		yyin = stdin;
	}
	yyparse();
}