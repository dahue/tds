#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <assert.h>
#include "symbol_table.h"

enum type func_type;

gboolean foo (GNode *node, gpointer data){
    struct Symbol *s = node->data;
    printf("%s\n", s->name);

    if (strcmp(s->name, "ASSIGN_OP") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        printf("names: %s %s\n", l->name, r->name);
        assert(l->type == r->type);
        s->type = l->type;
    }
    else if (strcmp(s->name, "+") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
        assert(l->type == E_INTEGER);
        s->type = E_INTEGER;
    }
    else if (strcmp(s->name, "*") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
        assert(l->type == E_INTEGER);
        s->type = E_INTEGER;
    }
    else if (strcmp(s->name, "AND") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
        assert(l->type == E_BOOL);
        s->type = E_BOOL;
    }
    else if (strcmp(s->name, "EQUAL") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
        s->type = E_BOOL;

    }
    else if (strcmp(s->name, "!") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        printf("types: %d\n", l->type);
        assert(l->type == E_BOOL);
        s->type = E_BOOL;
    }
    // else if (s->flag == E_FUNC){
    //     printf("func_name: %s\n", s->name);
    //     func_type = s->type;
    // }
    // else if (strcmp(s->name, "RETURN") == 0){
    //     struct Symbol *l = g_node_nth_child(node, 0)->data;
    //     printf("types: %d\n", l->type);
    //     assert(l->type == func_type);
    // }
    return false;
}

bool typeCheck(GNode *root){
    // struct Symbol *s = root->data;
    // printf("%s\n", s->name);

    g_node_traverse (root, G_POST_ORDER, G_TRAVERSE_ALL, -1, foo, NULL);
}