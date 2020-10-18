%{
#include <glib.h>
#include <stdbool.h>
#include "util/symbol_table.c"
#include "util/verbose.c"

int yylex();
void yyerror();

GList *stack = NULL;

GNode *tree;

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
%nonassoc EQUAL
%left '+' 
%left '*'
%left '!'

%%

main: {stack = newSymbolTable(stack);} program {tree = $2;};

program:
    var_decl_list {
        $$ = NULL;
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
        // printf("%s%d\n", "PARSER_TYPE: ",$1);
        int i;
        for (i=0; i < g_list_length($2); i++){
            char *name = (char*)g_list_nth_data($2, i);
            struct Symbol *s = newSymbol();
            s->flag = E_ID;
            s->name = name;
            s->type = $1;
            if (exists(stack, name) == NULL){
                // printf("%s%s\n", "PARSER_ID: ",name);
                stack = insertSymbol(stack, s);
            }
            else {
                printf("Variable '%s' already exists\n", name);
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
        $$=E_INTEGER;
    }
    | VOID {
        $$=E_VOID;
    }
    | BOOL {
        $$=E_BOOL;
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
        // printf("%s%d\n", "PARSER_TYPE: ",$1);
        // printf("%s%s\n", "PARSER_ID: ",$2);

        char* name = $2;
        struct Symbol *s = newSymbol();
        s->flag = E_FUNC;
        s->name = name;
        s->type = $1;
        s->param = $5;

        if (exists(stack, name) == NULL){
            // printf("%s%s\n", "PARSER_ID: ",name);
            stack = insertSymbol(stack, s);
        }
        else {
            printf("function '%s' already exists\n", name);
        }

        GNode *parent = g_node_new(s);
        g_node_append(parent, $7);
        $$ = parent;
    }
    | EXTERN type ID '(' {stack = addLevel(stack);} method_decl_params {stack = removeLevel(stack);} ')' SEMICOLON {
        // printf("%s%d\n", "PARSER_TYPE: ",$2);
        // printf("%s%s\n", "PARSER_ID: ",$3);

        char* name = $3;
        struct Symbol *s = newSymbol();
        s->flag = E_FUNC;
        s->name = name;
        s->type = $2;
        s->param = $6;

        if (exists(stack, name) == NULL){
            // printf("%s%s\n", "PARSER_ID: ",name);
            stack = insertSymbol(stack, s);
        }
        else {
            printf("function '%s' already exists\n", name);
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
            // printf("%s%d\n", "PARSER_TYPE: ",type);
            // printf("%s",toString(s));
            if (exists(stack, name) == NULL){
                // printf("%s%s\n", "PARSER_ID: ",name);
                stack = insertSymbol(stack, s);
            }          
            else{
                printf("Method '%s' already exists\n", name);
            }
        }
        $$ = stack;        
    }
    | {$$ = stack;}
    ;

method_decl_params_list:
    type ID{
        struct Symbol *s = newSymbol();
        s->flag = E_ID;
        s->name = $2;
        s->type = $1;
        GList *l = NULL;
        $$ = g_list_append(l, s);
    }
    | type ID COMMA method_decl_params_list{
        struct Symbol *s = newSymbol();
        s->flag = E_ID;
        s->name = $2;
        s->type = $1;
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
        GNode *first_child = g_node_new(findSymbol(stack, $1));
        struct Symbol *s = newSymbol();
        s->name = "ASSIGN_OP";
        GNode *parent = g_node_new(s);
        g_node_append(parent, first_child);
        g_node_append(parent, $3);
        $$ = parent;
    }
    | RETURN expression SEMICOLON {
        struct Symbol *s = newSymbol();
        s->name = "RETURN";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        $$ = parent;
    }
    | SEMICOLON {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        GNode *parent = g_node_new(s);
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
            printf("variable not defined.");
        }
        
    }
    | integer_literal{
        struct Symbol *s = newSymbol();
        int length = snprintf( NULL, 0, "%d", $1 );
        char* str = malloc( length + 1 );
        snprintf( str, length + 1, "%d", $1 );
        s->type = E_INTEGER;
        s->name = str;
        s->value_int = $1;
        $$ = g_node_new(s);
    }
    | bool_literal{
        struct Symbol *s = newSymbol();
        int length = snprintf( NULL, 0, "%d", $1 );
        char* str = malloc( length + 1 );
        snprintf( str, length + 1, "%d", $1 );
        s->type = E_BOOL;
        s->name = str;
        s->value_bool = $1;
        $$ = g_node_new(s);
    }
    | '!' expression {
        struct Symbol *s = newSymbol();
        s->name = "!";
        s->flag = E_FUNC;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);

        $$ = parent;
    }
    | expression AND expression {
        struct Symbol *s = newSymbol();
        s->name = "AND";
        s->flag = E_FUNC;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression EQUAL expression {
        struct Symbol *s = newSymbol();
        s->name = "EQUAL";
        s->flag = E_FUNC;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression '+' expression {
        struct Symbol *s = newSymbol();
        s->name = "+";
        s->flag = E_FUNC;
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression '*' expression {
        struct Symbol *s = newSymbol();
        s->name = "*";
        s->flag = E_FUNC;
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
        // printf("%d\n",$1);
    }
  ;

bool_literal:
    V_TRUE {
        $$=$1; 
        // printf("%d\n",$1);
    }
    | V_FALSE {
        $$=$1; 
        // printf("%d\n",$1);
    }
  ;
 
%%

GNode * returnAST(){
    return tree;
}