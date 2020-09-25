%{
#include "syntactic_analyzer.h"
%}

%option noyywrap
%option yylineno

integer 	("integer")
bool 		("bool")
false 		("false")
true 		("true")
extern 		("extern")
return 		("return")
void		("void")

and			("&&")
equal		("==")
assign_op 	("=")
alpha 		[a-zA-Z]
digit 		[0-9]
alpha_num 	({alpha}|{digit}|"_")
id	 		{alpha}({alpha_num})*
l_comment 	("//").*
sb_comment	("/*")
eb_comment	("*/")
comma 		(",")
semicolon	(";")


%%
{l_comment}					    { 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("L_COMMENT : %s\n",yytext);
								}

{sb_comment}					{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("SB_COMMENT : %s\n",yytext);
								}

{eb_comment}					{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("EB_COMMENT : %s\n",yytext);
								}

{integer}						{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("INTEGER : %s\n",yytext);
									return INTEGER;
								}

{bool}							{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("BOOL : %s\n",yytext);
									return BOOL;
								}

{false}							{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("FALSE : %s\n",yytext);
									return FALSE;
								}

{true}							{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("TRUE : %s\n",yytext);
									return TRUE;
								}

{extern}						{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("EXTERN : %s\n",yytext);
									return EXTERN;
								}

{return}						{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("RETURN : %s\n",yytext);
									return RETURN;
								}

{void}							{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("VOID : %s\n",yytext);
									return VOID;
								}

{equal}							{
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("EQUAL : %s\n",yytext);
									// return EQUAL;
								}

{and}							{
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("AND : %s\n",yytext);
									// return AND;
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

{id}					    	{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("ID : %s\n",yytext);
									return ID;
								}

{comma}					    	{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("COMMA : %s\n",yytext);
									return COMMA;
								}

{semicolon}					    	{ 
									yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
									strcpy(yylval.s,yytext);
									printf("SEMICOLON : %s\n",yytext);
									return SEMICOLON;
								}

[+*;(){}!]							{  
									printf("%s\n",yytext);
                                	return *yytext;
								}

(\n|" "|\t)                        /* ignore all the rest */

.									{printf("Error! %s\n",yytext);}
%%

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}
