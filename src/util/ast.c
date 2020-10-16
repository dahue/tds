#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <assert.h>
#include "symbol_table.h"

gboolean foo (GNode *node, gpointer data){
    struct Symbol *s = node->data;

    printf("%s\n", s->name);
    if (strcmp(s->name, "ASSIGN_OP") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
    }
    else if (strcmp(s->name, "+") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
    }
    else if (strcmp(s->name, "*") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
    }
    else if (strcmp(s->name, "AND") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
    }
    else if (strcmp(s->name, "EQUAL") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        printf("types: %d %d\n", l->type, r->type);
        assert(l->type == r->type);
    }
    return false;
}

bool typeCheck(GNode *root){
    g_node_traverse (root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, foo, NULL);
}