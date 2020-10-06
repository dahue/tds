%{
#include "util/symbol_table.h"
#include "util/verbose.c"

int yylex();
void yyerror();

%}

%locations
%union { 
  int i; 
  char *s;
}
 
%token<i> INT
%token<s> ID

%token<s> INTEGER
%token<s> BOOL
%token V_FALSE
%token V_TRUE
%token EXTERN
%token RETURN
%token VOID
%token EQUAL
%token AND
%token ASSIGN_OP
%token COMMA
%token SEMICOLON

%type<s> var_list
%type<s> type

%left AND
%nonassoc EQUAL
%left '+' 
%left '*'
%left '!'

 
%%
 
program: 
    var_decl_list
  | method_decl_list
  | var_decl_list method_decl_list
  |
  ;

var_decl_list:
    var_decl
  | var_decl_list var_decl
  ;

var_decl:
    type var_list SEMICOLON {

    }
  ;

var_list:
    ID {
      verbose("%s%s\n", "AS_ID: ",$1);
    }
  | var_list COMMA ID
  ;

type:
    INTEGER {
      verbose("%s%s\n", "AS_TYPE: ",$1);
    }
  | BOOL {
      verbose("%s%s\n", "AS_TYPE: ",$1);
    }
  ;

method_decl_list:
    method_decl
  | method_decl_list method_decl
  ;

method_decl:
    type ID '(' method_decl_params ')' block
  | VOID ID '(' method_decl_params ')' block
  | EXTERN type ID '(' method_decl_params ')' SEMICOLON
  | EXTERN VOID ID '(' method_decl_params ')' SEMICOLON
  ;

method_decl_params:
    type ID
  | type ID COMMA method_decl_params
  |
  ;

block:
    '{' '}'
  | '{' var_decl_list '}'
  | '{' statement_list '}'
  | '{' var_decl_list statement_list '}'
  ;

statement_list:
    statement
  | statement_list statement
  ;

statement:
    ID ASSIGN_OP expression SEMICOLON
  | RETURN expression SEMICOLON
  | SEMICOLON
  | block
  ;

expression:
    ID
  | literal
  | '!' expression
  | expression AND expression
  | expression EQUAL expression
  | expression '+' expression
  | expression '*' expression
  | '(' expression ')'
  ;

literal:
    integer_literal
  | bool_literal
  ;

integer_literal:
    INT
  ;

bool_literal:
    V_TRUE
  | V_FALSE
  ;
 
%%
