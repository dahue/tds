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

    struct node *tree = returnAST();
    printf("%s%d\n\n", "Resultado: ", evalPostorder(tree)->dataINT);
    printf("PSEUDO-ASSEMBLER\n");
    assemblerGen(tree);
}