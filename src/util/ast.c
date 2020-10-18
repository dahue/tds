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
    // printf("node: %s, flag: %d, type %d\n", s->name,s->flag, s->type);

    if (strcmp(s->name, "ASSIGN_OP") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        assert(l->type == r->type);
        s->type = l->type;
    }
    else if (strcmp(s->name, "+") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        assert(l->type == r->type);
        assert(l->type == E_INTEGER);
        s->type = E_INTEGER;
    }
    else if (strcmp(s->name, "*") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        assert(l->type == r->type);
        assert(l->type == E_INTEGER);
        s->type = E_INTEGER;
    }
    else if (strcmp(s->name, "AND") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        assert(l->type == r->type);
        assert(l->type == E_BOOL);
        s->type = E_BOOL;
    }
    else if (strcmp(s->name, "EQUAL") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        struct Symbol *r = g_node_nth_child(node, 1)->data;
        assert(l->type == r->type);
        s->type = E_BOOL;

    }
    else if (strcmp(s->name, "!") == 0){
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        assert(l->type == E_BOOL);
        s->type = E_BOOL;
    }
    else if (strcmp(s->name, "RETURN") == 0){
        struct Symbol *p = ((node->parent)->parent)->data;
        struct Symbol *l = g_node_nth_child(node, 0)->data;
        printf("parent node: %s, flag: %d, type %d\n", p->name, p->flag, p->type);
        printf("child node: %s, flag: %d, type %d\n", l->name, l->flag, l->type);
        assert(p->type == l->type);
        s->type = l->type;
    }
    printf("node: %s, flag: %d, type %d\n", s->name,s->flag, s->type);
    return false;
}

bool typeCheck(GNode *root){
    g_node_traverse (root, G_POST_ORDER, G_TRAVERSE_ALL, -1, foo, NULL);
}