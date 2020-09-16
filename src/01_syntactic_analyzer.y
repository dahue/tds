%{
#include "util/TS.c"
#include "util/ast.c"

int yylex();
void yyerror();

struct node *tree;
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

%type<n> expr
 
%left '+' 
%left '*'
 
%%
 
prog: 
  declaracion expr ';'          { 
                                  tree = $2;
                                }
  ;

declaracion:
  VAR_DECL ID ASSIGN_OP INT ';' declaracion { 
                                              if (existe($2)==0) insertar($2, $4);
                                              else  printf("%s%s\n", "Variable redeclarada :",$2);
                                            }
  |
  ;

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
                      }
  | expr '+' expr     {
                        $$ = newNodeSTR("+", $1, $3);
                      }
  | expr '*' expr     {
                        $$ = newNodeSTR("*", $1, $3);
                      }
  | '(' expr ')'      { $$ =  $2; }
  ;
 
%%

struct node *returnAST(){
  return tree;
}
