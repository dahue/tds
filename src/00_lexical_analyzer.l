%{
#include "syntactic_analyzer.h"
%}

%option noyywrap
%option yylineno

var_decl ("var"|"VAR")
assign_op =
letter [a-zA-z]
digit [0-9]
ident {letter}({letter}|{digit})*

%%
{var_decl}						{
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("VAR_DECL : %s\n",yytext);
									return VAR_DECL;
								}

{assign_op}						{
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("ASSIGN_OP : %s\n",yytext);
									return ASSIGN_OP;
								}

{digit}+                     	{ 
									yylval.i = atoi(yytext);
									printf("INT : %d\n",atoi(yytext)); 
									return INT;
								}

{ident}					    	{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("ID : %s\n",yytext);
									return ID;
								}

[+*;()]							{  
									printf("%s\n",yytext);
                                	return *yytext;
								}

.|\n                          	; /* ignore all the rest */
%%

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}
