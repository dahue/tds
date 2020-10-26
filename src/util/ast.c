#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <assert.h>
#include "symbol_table.h"

enum type func_type;
int return_count;
int main_count;

gboolean post_order_analysis (GNode *node, gpointer data){
    struct Symbol *s = node->data;
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
        if (s->type != E_VOID){
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            s->type = l->type;
        }
    }
    printf("node: %s, flag: %d, type %d\n", s->name,s->flag, s->type);
    return false;
}

gboolean symcmp (GNode *node, gpointer data){
    struct Symbol *s = node->data;
    struct Symbol *r = (struct Symbol*)data;
    if (strcmp(s->name, r->name) == 0){
        return_count += 1;
        return true;
    }
    return false;
}

gboolean pre_order_analysis (GNode *node, gpointer data){
    struct Symbol *s = node->data;
    if (s->flag == E_FUNC){
        printf("node: %s, flag: %d, type %d\n", s->name,s->flag, s->type);
        if (strcmp(s->name, "main") == 0){
            assert(g_hash_table_size((GHashTable*)((GList*)s->param)->data) == 0);
            main_count += 1;
        }
        struct Symbol *r = newSymbol();
        r->name = "RETURN";
        return_count = 0;
        g_node_traverse(node, G_PRE_ORDER, G_TRAVERSE_ALL, -1, symcmp, r);
        assert(return_count > 0);
        func_type = s->type;
    }
    else if (strcmp(s->name, "RETURN") == 0){
        printf("node: %s, flag: %d, type %d\n", s->name,s->flag, s->type);
        assert(func_type == s->type);
    }
    return false;
}

bool typeCheck(GNode *root){
    printf("\npost_order_analysis\n");
    g_node_traverse (root, G_POST_ORDER, G_TRAVERSE_ALL, -1, post_order_analysis, NULL);

    printf("\npre_order_analysis\n");
    g_node_traverse (root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, pre_order_analysis, NULL);

    assert(main_count > 0);
}