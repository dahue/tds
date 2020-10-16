%{
#include <assert.h>
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

main: {stack = newSymbolTable(stack);} program;

program:
    var_decl_list {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        $$ = parent;
    }
    | method_decl_list {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        $$ = parent;
    }
    | var_decl_list method_decl_list {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $2);
        $$ = parent;
    }
  ;

var_decl_list:
    var_decl {
        struct Symbol *s = newSymbol();
        s->name = "SEMICOLON";
        GNode *parent = g_node_new(s);
        GList *l = g_hash_table_get_values($1->data);
        int i;
        struct Symbol *e = NULL;
        for (i=0; i< g_list_length(l); i++){
            e = (struct Symbol*)g_list_nth_data(l, i);
            g_node_append(parent, g_node_new(e));
        }
        $$ = parent;
    }
    | var_decl_list var_decl {
        GNode *parent = $1;
        GList *l = g_hash_table_get_values($2->data);
        int i;
        struct Symbol *e = NULL;
        for (i=0; i< g_list_length(l); i++){
            e = (struct Symbol*)g_list_nth_data(l, i);
            g_node_append(parent, g_node_new(e));
        }
        $$ = parent;
    }
  ;

var_decl:
    type var_list SEMICOLON {
        printf("%s%d\n", "PARSER_TYPE: ",$1);
        int i;
        for (i=0; i < g_list_length($2); i++){
            char *name = (char*)g_list_nth_data($2, i);
            struct Symbol *s = newSymbol();
            s->flag = E_ID;
            s->name = name;
            s->type = $1;
            if (exists(stack, name) == NULL){
                printf("%s%s\n", "PARSER_ID: ",name);
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
        s->name = "SEMICOLON";
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
        printf("%s%d\n", "PARSER_TYPE: ",$1);
        printf("%s%s\n", "PARSER_ID: ",$2);

        char* name = $2;
        struct Symbol *s = newSymbol();
        s->flag = E_FUNC;
        s->name = name;
        s->type = $1;
        s->param = $5;

        if (exists(stack, name) == NULL){
            printf("%s%s\n", "PARSER_ID: ",name);
            stack = insertSymbol(stack, s);
        }
        else {
            printf("function '%s' already exists\n", name);
        }

        GNode *parent = g_node_new(s);
        g_node_append(parent, $7);
        $$ = parent;
    }
    // | VOID ID '(' {stack = addLevel(stack);} method_decl_params ')' block {stack = removeLevel(stack);
    //     printf("%s%s\n", "PARSER_TYPE: ",$1);
    //     printf("%s%s\n", "PARSER_ID: ",$2);

    //     char* name = $2;
    //     struct Symbol *s = newSymbol();
    //     s->flag = E_FUNC;
    //     s->name = name;
    //     s->type = E_VOID;
    //     s->param = $5;

    //     if (exists(stack, name) == NULL){
    //         printf("%s%s\n", "PARSER_ID: ",name);
    //         stack = insertSymbol(stack, s);
    //     }
    //     else {
    //         printf("function '%s' already exists\n", name);
    //     }

    //     GNode *parent = g_node_new(s);
    //     g_node_append(parent, $7);
    //     $$ = parent;
    // }
    | EXTERN type ID '(' {stack = addLevel(stack);} method_decl_params {stack = removeLevel(stack);} ')' SEMICOLON {
        printf("%s%d\n", "PARSER_TYPE: ",$2);
        printf("%s%s\n", "PARSER_ID: ",$3);

        char* name = $3;
        struct Symbol *s = newSymbol();
        s->flag = E_FUNC;
        s->name = name;
        s->type = $2;
        s->param = $6;

        if (exists(stack, name) == NULL){
            printf("%s%s\n", "PARSER_ID: ",name);
            stack = insertSymbol(stack, s);
        }
        else {
            printf("function '%s' already exists\n", name);
        }

        GNode *parent = g_node_new(s);
        $$ = parent;
    }
    // | EXTERN VOID ID '(' {stack = addLevel(stack);} method_decl_params {stack = removeLevel(stack);} ')' SEMICOLON {
    //     printf("%s%s\n", "PARSER_TYPE: ",$2);
    //     printf("%s%s\n", "PARSER_ID: ",$3);

    //     char* name = $3;
    //     struct Symbol *s = newSymbol();
    //     s->flag = E_FUNC;
    //     s->name = name;
    //     s->type = E_VOID;
    //     s->param = $6;

    //     if (exists(stack, name) == NULL){
    //         printf("%s%s\n", "PARSER_ID: ",name);
    //         stack = insertSymbol(stack, s);
    //     }
    //     else {
    //         printf("function '%s' already exists\n", name);
    //     }

    //     GNode *parent = g_node_new(s);
    //     $$ = parent;
    // }
    ;

method_decl_params:
    method_decl_params_list {
        int i;
        for (i=0; i < g_list_length($1); i++){
            struct Symbol *s = (struct Symbol *)g_list_nth_data($1, i);
            char *name = s->name;
            enum type type = s->type;
            printf("%s%d\n", "PARSER_TYPE: ",type);
            // printf("%s",toString(s));
            if (exists(stack, name) == NULL){
                printf("%s%s\n", "PARSER_ID: ",name);
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
        struct Symbol *s = newSymbol();
        s->name = "BLOCK";
        GNode *parent = g_node_new(s);
        $$ = parent;
    }
    | open_brace var_decl_list close_brace {
        struct Symbol *s = newSymbol();
        s->name = "BLOCK";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        $$ = parent;
    }
    | open_brace statement_list close_brace {
        struct Symbol *s = newSymbol();
        s->name = "BLOCK";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        $$ = parent;
    }
    | open_brace var_decl_list statement_list close_brace{
        struct Symbol *s = newSymbol();
        s->name = "BLOCK";
        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);
        g_node_append(parent, $3);
        $$ = parent;
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
        struct Symbol *s = newSymbol();
        s->name = "ASSIGN_OP";
        GNode *first_child = g_node_new(findSymbol(stack, $1));
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
        // struct Symbol *s = findSymbol(stack, newSymbol(E_ID, $1, E_ID));
        // $$ = g_node_new (s);
        // printf("flag: %d, type: %d, name: %s, value: %d, param %s\n", s->flag, s->type, s->name, s->value, s->param);
        // if (s != NULL) 
        //     $$ = g_node_new(s);
        // else 
        //     printf("Variable '%s' no declarada \n",$1);

        struct Symbol *s = findSymbol(stack, $1);
        $$ = g_node_new(s);
    }
    | integer_literal{
        struct Symbol *s = newSymbol();
        s->type = E_INTEGER;
        s->value_int = $1;
        $$ = g_node_new(s);
    }
    | bool_literal{
        // printf("%d\n", $1);
        struct Symbol *s = newSymbol();
        s->type = E_BOOL;
        s->value_bool = $1;
        $$ = g_node_new(s);
    }
    | '!' expression {
        // struct Symbol *a = $2->data;
        // assert(a->type == E_BOOL);
        struct Symbol *s = newSymbol();
        s->type = E_BOOL;
        s->flag = E_FUNC;
        s->name = "!";

        GNode *parent = g_node_new(s);
        g_node_append(parent, $2);

        $$ = parent;

        // struct Symbol *p = $$->data;
        // struct Symbol *a = g_node_nth_child($$, 0)->data;
        // printf("%s %d\n",p->name, a->value_bool);
    }
    | expression AND expression {
        struct Symbol *s = newSymbol();
        s->type = E_BOOL;
        s->flag = E_FUNC;
        s->name = "AND";

        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression EQUAL expression {
        struct Symbol *s = newSymbol();
        s->type = E_BOOL;
        s->flag = E_FUNC;
        s->name = "EQUAL";

        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
    }
    | expression '+' expression {
        // struct Symbol *a = $1->data;
        // struct Symbol *b = $3->data;
        // assert(a->type == b->type);
        // assert(a->type == E_INTEGER);

        struct Symbol *s = newSymbol();
        s->type = E_INTEGER;
        s->flag = E_FUNC;
        s->name = "+";

        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
        
        // struct Symbol *p = $$->data;
        // a = g_node_nth_child($$, 0)->data;
        // b = g_node_nth_child($$, 1)->data;
        // printf("%d %s %d\n",a->value_int, p->name, b->value_int);

    }
    | expression '*' expression {
        // struct Symbol *a = $1->data;
        // struct Symbol *b = $3->data;
        // assert(a->type == b->type);
        // assert(a->type == E_INTEGER);
        struct Symbol *s = newSymbol();
        s->type = E_INTEGER;
        s->flag = E_FUNC;
        s->name = "*";

        GNode *parent = g_node_new(s);
        g_node_append(parent, $1);
        g_node_append(parent, $3);

        $$ = parent;
        
        // struct Symbol *p = $$->data;
        // a = g_node_nth_child($$, 0)->data;
        // b = g_node_nth_child($$, 1)->data;
        // printf("%d %s %d\n",a->value_int, p->name, b->value_int);

    }
    | '(' expression ')' {
        $$ = $2;
    }
  ;

integer_literal:
    INT {$$=$1; printf("%d\n",$1);}
  ;

bool_literal:
    V_TRUE {$$=$1; printf("%d\n",$1);}
    | V_FALSE {$$=$1; printf("%d\n",$1);}
  ;
 
%%

