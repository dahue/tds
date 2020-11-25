%{
#include <glib.h>
#include <stdbool.h>
#include "syntactic_analyzer.h"
#include "util/verbose.h"
%}

%option noyywrap
%option yylineno
%x C_COMMENT

integer     ("integer")
bool        ("bool")
false       ("false")
true        ("true")
extern      ("extern")
return      ("return")
void        ("void")
if          ("if")
then        ("then")
else        ("else")
while        ("while")
and			("&&")
equal		("==")
less        ("<")
assign_op 	("=")
alpha 		[a-zA-Z]
digit 		[0-9]
alpha_num 	({alpha}|{digit}|"_")
id	 		{alpha}({alpha_num})*
l_comment 	("//").*
comma 		(",")
semicolon	(";")


%%

"/*" {
    BEGIN(C_COMMENT); 
}
<C_COMMENT>"*/" { 
    BEGIN(INITIAL);
}
<C_COMMENT>.
<C_COMMENT>\n

{l_comment} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("L_COMMENT : %s\n",yytext);
}

{integer}						{ 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("INTEGER : %s\n",yytext);
    return INTEGER;
}

{bool} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("BOOL : %s\n",yytext);
    return BOOL;
}

{false}	{ 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("V_FALSE : %s\n",yytext);
    return V_FALSE;
}

{true} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("V_TRUE : %s\n",yytext);
    return V_TRUE;
}

{extern} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("EXTERN : %s\n",yytext);
    return EXTERN;
}

{return} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("RETURN : %s\n",yytext);
    return RETURN;
}

{void} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("VOID : %s\n",yytext);
    return VOID;
}

{if} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("IF : %s\n",yytext);
    return IF;
}

{then} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("THEN : %s\n",yytext);
    return THEN;
}

{else} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("ELSE : %s\n",yytext);
    return ELSE;
}

{while} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("WHILE : %s\n",yytext);
    return WHILE;
}

{equal} {
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("EQUAL : %s\n",yytext);
    return EQUAL;
}

{less} {
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("LESS : %s\n",yytext);
    return LESS;
}

{and} {
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("AND : %s\n",yytext);
    return AND;
}

{assign_op} {
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("ASSIGN_OP : %s\n",yytext);
    return ASSIGN_OP;
}

{digit}+ { 
    yylval.i = atoi(yytext);
    verbose("INT : %d\n",atoi(yytext)); 
    return INT;
}

{id} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("ID : %s\n",yytext);
    return ID;
}

{comma} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("COMMA : %s\n",yytext);
    return COMMA;
}

{semicolon} { 
    yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
    strcpy(yylval.s,yytext);
    verbose("SEMICOLON : %s\n",yytext);
    return SEMICOLON;
}

[+*;()\[\]{}!] {  
    verbose("%s\n",yytext);
    return *yytext;
}

(\n|\r\n|" "|\t) /* ignore all the rest */

. {
    printf("tdsc> lexical error: '%s' in line %d.\n", yytext, yylineno);
    exit(0);
}

%%

void yyerror (char *msg) {
  printf("tdsc> %s: '%s' in line %d.\n", msg, yytext, yylineno);
  exit(0);
}

