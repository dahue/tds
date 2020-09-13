%{

#include <stdlib.h>
#include <stdio.h>
#include "TS.c"
#include "ast.c"

int yylex();
void yyerror();

%}
 
%union { 
  int i; 
  char *s; 
  struct node *n;
}
 
%token<i> INT
%token<s> ID
%token VAR_DECL
%token ASSIGN_OP
%token EOS

%type<n> expr
 
%left '+' 
%left '*'
 
%%
 
prog: 
  declaracion expr ';'          { 
                                  printf("%s%d\n\n", "Resultado: ", evalPostorder($2)->dataINT);
                                  printf("PSEUDO-ASSEMBLER\n");
                                  assemblerGen($2);
                                }
  ;

// declaraciones: 
//   declaracion
//   | declaracion declaraciones
//   ;

declaracion:
  VAR_DECL ID ASSIGN_OP INT ';' declaracion { 
                                              if (existe($2)==0) insertar($2, $4);
                                              else  printf("%s%s\n", "Variable redeclarada :",$2);
                                            }
  |
  ;

// asignacion:

//   ;

expr:
  ID                  { 
                        if (existe($1)==1){
                          $$ = newNodeINT(buscar_valor($1), NULL, NULL);
                        }
                        else{
                          printf("%s%s\n", "Variable no declarada :",$1);
                          $$ = newNodeINT(0, NULL, NULL);
                        }
                      }
  | INT               { // $$ = $1;
                        $$ = newNodeINT($1, NULL, NULL);
                        printf("%s%d\n","Constante entera:", $$->dataINT);
                        printf("%s%s\n","Tipo:",$$->type);
                      }
  | expr '+' expr     { // $$ = $1 + $3; 
                        $$ = newNodeSTR("+", $1, $3);
                        // printf("%s%s\n","Operador Suma: ", $$->dataSTR);
                      }
  | expr '*' expr     { // $$ = $1 * $3;
                        $$ = newNodeSTR("*", $1, $3);
                        // printf("%s%s\n","Operador Producto: ", $$->dataSTR);
                      }
  | '(' expr ')'      { $$ =  $2; }
  ;
 
%%


