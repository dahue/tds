%{

#include <stdlib.h>
#include <stdio.h>
#include "TS.c"
#include "binary_tree.c"

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

%type<i> expr
 
%left '+' 
%left '*'
 
%%
 
prog: 
  declaracion expr ';'          { printf("%s%d\n", "Resultado: ",$2); }
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
                        if (existe($1)==1) $$ = buscar_valor($1);
                        else{
                          printf("%s%s\n", "Variable no declarada :",$1);
                          $$ = 0;
                        }
                      }
  | INT               { // $$ = $1;
                        $$ = $1;
                        printf("%s%d\n","Constante entera:",$1);
                      }              
  | expr '+' expr     { $$ = $1 + $3; 
                        // printf("%s,%d,%d,%d\n","Operador Suma\n",$1,$3,$1+$3);
                      }
  | expr '*' expr     { $$ = $1 * $3; 
                        // printf("%s,%d,%d,%d\n","Operador Producto\n",$1,$3,$1*$3);  
                      }
  | '(' expr ')'      { $$ =  $2; 
                      }
  ;
 
%%


