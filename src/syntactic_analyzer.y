%{
#include <glib.h>
#include <stdbool.h>
#include "util/symbol_table.c"
#include "util/verbose.c"

int yylex();
void yyerror();

GList *stack = NULL;

GNode *tree;
extern int yylineno;
%}

%locations
%union { 
    bool b;
    GList *l;
    GNode *n;
    int i;
    char *s;
}
 
%token<i> INT
%token<b> V_FALSE
%token<b> V_TRUE
%token<s> ID
%token<s> VOID
%token<s> INTEGER
%token<s> BOOL
%token IF
%token THEN
%token ELSE
%token WHILE
%token LESS
%token EXTERN
%token RETURN
%token EQUAL
%token AND
%token ASSIGN_OP
%token COMMA
%token SEMICOLON

%type<l> var_list
%type<l> method_decl_params_list
%type<l> method_decl_params
%type<s> open_brace
%type<s> close_brace
%type<b> bool_literal
%type<i> integer_literal
%type<i> type
%type<n> statement
%type<n> statement_list
%type<n> expression
%type<n> block
%type<n> var_decl_list
%type<l> var_decl
%type<n> method_decl_list
%type<n> method_decl
%type<n> program

%left AND
%nonassoc LESS
%nonassoc EQUAL
%left '+' 
%left '*'
%left '!'

%%

main: {stack = newSymbolTable(stack);} program {tree = $2;};

program:
    var_decl_list {
        $$ = g_node_new(newSymbol());
    }
    | method_decl_list {
        struct Symbol *s = newSymbol();
        s->name = "METHOD_DECL_LIST";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        $$ = parent;
    }
    | var_decl_list method_decl_list {
        struct Symbol *s = newSymbol();
        s->name = "VAR_METHOD_DECL_LIST";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        $$ = parent;
    }
  ;

var_decl_list:
    var_decl {
    }
    | var_decl_list var_decl {
    }
  ;

var_decl:
    type var_list SEMICOLON {
        int i;
        for (i=0; i < g_list_length($2); i++){
            char *name = (char*)g_list_nth_data($2, i);
            struct Symbol *s = newSymbol();
            int stack_level = g_list_length(stack);
            if (stack_level == 1){
                s->flag = F_ID_GLOBAL;
            }
            else{
                s->flag = F_ID;
            }
            s->name = name;
            s->type = $1;
            s->lineno = yylineno;
            if (exists(stack, name) == NULL){
                stack = insertSymbol(stack, s);
            }
            else {
                printf("tdsc> error in line %d: variable '%s' already exists.\n", yylineno, name);
                exit(0);
            }
        }
        $$ = stack;
    }
    ;

var_list:
    ID {
        GList *l = NULL;
        $$ = g_list_append(l, $1);
    }
    | var_list COMMA ID {
        $$ = g_list_append($1, $3);
    }
  ;

type:
    INTEGER {
        $$=T_INTEGER;
    }
    | VOID {
        $$=T_VOID;
    }
    | BOOL {
        $$=T_BOOL;
    }
    ;

method_decl_list:
    method_decl {
        struct Symbol *s = newSymbol();
        s->name = "METHOD_DECL";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        $$ = parent;
    }
    | method_decl_list method_decl {
        GNode *parent = $1;
        g_node_append(parent, $2);
        $$ = parent;
    }
    ;

method_decl:
    type ID '(' {stack = addLevel(stack);} method_decl_params ')' block {stack = removeLevel(stack);
        char* name = $2;
        struct Symbol *s = newSymbol();
        s->flag = F_FUNC;
        s->name = name;
        s->type = $1;
        s->param = $5;
        s->lineno = yylineno;
        if (exists(stack, name) == NULL){
            stack = insertSymbol(stack, s);
        }
        else {
            printf("tdsc> error in line %d: function '%s' already exists.\n", yylineno, name);
            exit(0);
        }

        GNode *parent = g_node_new(s);
        g_node_append(parent, $7);
        $$ = parent;
    }
    | EXTERN type ID '(' {stack = addLevel(stack);} method_decl_params {stack = removeLevel(stack);} ')' SEMICOLON {
        char* name = $3;
        struct Symbol *s = newSymbol();
        s->flag = F_INDF;
        s->name = name;
        s->type = $2;
        s->param = $6;
        s->lineno = yylineno;
        if (exists(stack, name) == NULL){
            stack = insertSymbol(stack, s);
        }
        else {
            printf("tdsc> error in line %d: function '%s' already exists.\n", yylineno, name);
            exit(0);
        }

        GNode *parent = g_node_new(s);
        $$ = parent;
    }
    ;

method_decl_params:
    method_decl_params_list {
        int i;
        for (i=0; i < g_list_length($1); i++){
            struct Symbol *s = (struct Symbol *)g_list_nth_data($1, i);
            char *name = s->name;
            enum type type = s->type;
            if (exists(stack, name) == NULL){
                stack = insertSymbol(stack, s);
            }          
            else{
                printf("tdsc> error in line %d: parameter '%s' already exists.\n", yylineno, name);
                exit(0);
            }
        }
        $$ = stack;        
    }
    | {$$ = stack;}
    ;

method_decl_params_list:
    type ID{
        struct Symbol *s = newSymbol();
        s->flag = F_ID;
        s->name = $2;
        s->type = $1;
        s->lineno = yylineno;
        GList *l = NULL;
        $$ = g_list_append(l, s);
    }
    | type ID COMMA method_decl_params_list{
        struct Symbol *s = newSymbol();
        s->flag = F_ID;
        s->name = $2;
        s->type = $1;
        s->lineno = yylineno;
        $$ = g_list_append($4, s);
    }
    ;

block:
    open_brace close_brace {
        $$ = g_node_new(newSymbol());
    }
    | open_brace var_decl_list close_brace {
        $$ = g_node_new(newSymbol());
    }
    | open_brace statement_list close_brace {
        $$ = $2;
    }
    | open_brace var_decl_list statement_list close_brace{
        $$ = $3;
    }
  ;

open_brace:
    '{' {stack = addLevel(stack);}
    ;

close_brace:
    '}' {stack = removeLevel(stack);}
    ;

statement_list:
    statement {
        struct Symbol *s = newSymbol();
        s->name = "STATEMENT_LIST";
        s->flag = F_STATEMENT_LIST;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        $$ = parent;
    }
    | statement_list statement {
        g_node_append($1, $2);
        $$ = $1;
    }
  ;

statement:
    ID ASSIGN_OP expression SEMICOLON {
        struct Symbol *r = findSymbol(stack, $1);
        if (r != NULL) {
            GNode *first_child = g_node_new(r);
            struct Symbol *s = newSymbol();
            s->name = "ASSIGN_OP";
            s->flag = F_ASSIGN_OP;
            s->lineno = yylineno;
            GNode *parent = g_node_new(s);
            g_node_append(parent, first_child);
            g_node_append(parent, $3);
            $$ = parent;
        }
        else{
            printf("tdsc> error in line %d: variable '%s' not defined.\n", yylineno, $1);
            exit(0);
        }
    }
    | RETURN expression SEMICOLON {
        struct Symbol *s = newSymbol();
        s->name = "RETURN";
        s->type = T_INDF;
        s->flag = F_RETURN;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        $$ = parent;
    }
    | RETURN SEMICOLON {
        struct Symbol *s = newSymbol();
        s->name = "RETURN";
        s->type = T_VOID;
        s->lineno = yylineno;
        s->flag = F_RETURN;
        GNode *parent = g_node_new(s);
        $$ = parent;
    }
    | SEMICOLON {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        $$ = parent;
    }
    | IF '(' expression ')' THEN block {
        struct Symbol *s = newSymbol();
        s->name = "IF";
        s->type = T_INDF;
        s->flag = F_IF;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $3);
        g_node_append(parent, $6);
        $$ = parent;
    }
    | IF '(' expression ')' THEN block ELSE block {
        struct Symbol *s = newSymbol();
        s->name = "IF_ELSE";
        s->type = T_INDF;
        s->flag = F_IF_ELSE;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $3);
        g_node_append(parent, $6);
        g_node_append(parent, $8);
        $$ = parent;
    }
    | WHILE '(' expression ')' block {
        struct Symbol *s = newSymbol();
        s->name = "WHILE";
        s->type = T_INDF;
        s->flag = F_WHILE;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $3);
        g_node_append(parent, $5);
        $$ = parent;
    }
    | block {
        $$ = $1;
    }
  ;

expression:
    ID {
        struct Symbol *s = findSymbol(stack, $1);
        if (s != NULL){
            $$ = g_node_new(s);
        }
        else{
            printf("tdsc> error in line %d: variable '%s' not defined.\n", yylineno, $1);
            exit(0);
        }
    }
    | ID '(' ')' {
        struct Symbol *r = findSymbol(stack, $1);
        if (r != NULL) {
            GList *param;
            struct Symbol *s = newSymbol();
            s->name = r->name;
            s->type = r->type;
            s->flag = F_FUNC_CALL;
            s->param = r->param;
            s->lineno = yylineno;
            GNode *parent = g_node_new(s);
            $$ = parent;
        }
        else{
            printf("tdsc> error in line %d: function '%s' not defined.\n", yylineno, $1);
            exit(0);
        }
    }
    | ID '(' ID ')' {
        struct Symbol *r = findSymbol(stack, $1);
        if (r != NULL) {
            GList *param;
            param = g_list_append(param, $3);
            struct Symbol *s = newSymbol();
            s->name = r->name;
            s->type = r->type;
            s->flag = F_FUNC_CALL;
            s->param = r->param;
            s->lineno = yylineno;
            GNode *parent = g_node_new(s);
            $$ = parent;
        }
        else{
            printf("tdsc> error in line %d: function '%s' not defined.\n", yylineno, $1);
            exit(0);
        }
    }
    | ID '(' ID COMMA ID ')' {
        struct Symbol *r = findSymbol(stack, $1);
        if (r != NULL) {
            GList *param;
            param = g_list_append(param, $3);
            param = g_list_append(param, $5);
            struct Symbol *s = newSymbol();
            s->name = r->name;
            s->type = r->type;
            s->flag = F_FUNC_CALL;
            s->param = r->param;
            s->lineno = yylineno;
            GNode *parent = g_node_new(s);
            $$ = parent;
        }
        else{
            printf("tdsc> error in line %d: function '%s' not defined.\n", yylineno, $1);
            exit(0);
        }
    }
    | integer_literal{
        struct Symbol *s = newSymbol();
        int length = snprintf( NULL, 0, "%d", $1 );
        char* str = malloc( length + 1 );
        snprintf( str, length + 1, "%d", $1 );
        s->type = T_INTEGER;
        s->name = str;
        s->flag = F_CONST;
        s->value_int = $1;
        s->lineno = yylineno;
        $$ = g_node_new(s);
    }
    | bool_literal{
        struct Symbol *s = newSymbol();
        int length = snprintf( NULL, 0, "%d", $1 );
        char* str = malloc( length + 1 );
        snprintf( str, length + 1, "%d", $1 );
        s->type = T_BOOL;
        s->name = str;
        s->flag = F_CONST;
        s->value_bool = $1;
        s->lineno = yylineno;
        $$ = g_node_new(s);
    }
    | '!' expression {
        struct Symbol *s = newSymbol();
        s->name = "!";
        s->flag = F_NOT_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);

        $$ = parent;
    }
    | expression AND expression {
        struct Symbol *s = newSymbol();
        s->name = "AND";
        s->flag = F_AND_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression EQUAL expression {
        struct Symbol *s = newSymbol();
        s->name = "EQUAL";
        s->flag = F_EQ_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression LESS expression {
        struct Symbol *s = newSymbol();
        s->name = "LESS";
        s->flag = F_LESS_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression '+' expression {
        struct Symbol *s = newSymbol();
        s->name = "+";
        s->flag = F_PLUS_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression '*' expression {
        struct Symbol *s = newSymbol();
        s->name = "*";
        s->flag = F_MUL_OP;
        s->lineno = yylineno;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | '(' expression ')' {
        $$ = $2;
    }
  ;

integer_literal:
    INT {
        $$=$1; 
    }
  ;

bool_literal:
    V_TRUE {
        $$=true; 
    }
    | V_FALSE {
        $$=false; 
    }
  ;
 
%%

GNode * returnAST(){
    return tree;
}