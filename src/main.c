#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntactic_analyzer.c"
#include "util/verbose.h"
#include "util/ast.c"
#include "util/tac.c"
#include "util/asm.c"

extern FILE * yyin;

int main(int argc,char *argv[]){
	++argv,--argc;
	char *file_path;
	int skip = 0;
	if (argc == 1) {
		yyin = fopen(argv[0],"r");
		file_path = "o.s";
	}
	else if (argc >= 2)
	{
		if (strcmp(argv[0], "-v") == 0) {
			setVerbose(true);
			yyin = fopen(argv[1],"r");
		}
		else if (strcmp(argv[0], "-o") == 0) {
			file_path = argv[1];
			yyin = fopen(argv[2],"r");
		}
		else if (strcmp(argv[0], "-s") == 0) {
			skip = 1;
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

	GNode *tree = returnAST();
	typeCheck(tree);
	// printf("\nThree Address Code\n");
	GList *three_address_code = tac(tree);

	if (skip != 1){
		FILE *fp = fopen (file_path, "w");
		generate_assembler_code(three_address_code, fp);
		fclose(fp);
	}
}