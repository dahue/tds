#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "symbol_table.h"

enum type func_type;
int return_count;
int main_count;

void error(char *msg, int lineno){
    printf("tdsc> error in line %d. %s.\n", lineno, msg);
    exit(0);
}

gboolean post_order_analysis (GNode *node, gpointer data){
    struct Symbol *s = node->data;
    struct Symbol *l;
    struct Symbol *r;
    bool cond;
    switch(s->flag) {
        case F_ASSIGN_OP:
            l = g_node_nth_child(node, 0)->data;
            r = g_node_nth_child(node, 1)->data;
            cond = l->type == r->type;
            cond ? : error("types don't match", s->lineno);
            s->type = l->type;
            break;
        case F_PLUS_OP:
            l = g_node_nth_child(node, 0)->data;
            r = g_node_nth_child(node, 1)->data;
            cond = (l->type == r->type) && (l->type == T_INTEGER);
            cond ? : error("types don't match", s->lineno);
            s->type = T_INTEGER;
            break;
        case F_MUL_OP:
            l = g_node_nth_child(node, 0)->data;
            r = g_node_nth_child(node, 1)->data;
            cond = (l->type == r->type) && (l->type == T_INTEGER);
            cond ? : error("types don't match", s->lineno);
            s->type = T_INTEGER;
            break;
        case F_AND_OP:
            l = g_node_nth_child(node, 0)->data;
            r = g_node_nth_child(node, 1)->data;
            cond = (l->type == r->type) && (l->type == T_BOOL);
            cond ? : error("types don't match", s->lineno);
            s->type = T_BOOL;
            break;
        case F_EQ_OP:
            l = g_node_nth_child(node, 0)->data;
            r = g_node_nth_child(node, 1)->data;
            cond = (l->type == r->type);
            cond ? : error("types don't match", s->lineno);
            s->type = T_BOOL;
            break;
        case F_NOT_OP:
            l = g_node_nth_child(node, 0)->data;
            cond = (l->type == T_BOOL);
            cond ? : error("types don't match", s->lineno);
            s->type = T_BOOL;
            break;
        case F_RETURN:
            if (s->type != T_VOID){
                l = g_node_nth_child(node, 0)->data;
                s->type = l->type;
            }
            break;
    }
    // printf("node: %s, flag: %d, type %d\n", s->name, s->flag, s->type);
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
    bool cond;
    // printf("node: %s, flag: %d, type %d\n", s->name, s->flag, s->type);
    switch(s->flag){
        case F_FUNC:
            // printf("node: %s, flag: %d, type %d\n", s->name, s->flag, s->type);
            if (strcmp(s->name, "main") == 0){
                cond = (g_hash_table_size((GHashTable*)((GList*)s->param)->data) == 0);
                cond ? : error("parameters are not allowed in main method", s->lineno);
                main_count += 1;
            }
            struct Symbol *r = newSymbol();
            r->name = "RETURN";
            return_count = 0;
            g_node_traverse(node, G_PRE_ORDER, G_TRAVERSE_ALL, -1, symcmp, r);
            cond = (return_count > 0);
            cond ? : error("return statement missing", s->lineno);
            func_type = s->type;
            break;
        case F_RETURN:
            // printf("node: %s, flag: %d, type %d, function type %d\n", s->name, s->flag, s->type, func_type);
            cond = (func_type == s->type);
            cond ? : error("return type doesn't match with the function definition", s->lineno);
            break;
    }
    return false;
}

bool typeCheck(GNode *root){
    // printf("\npost_order_analysis\n");
    g_node_traverse (root, G_POST_ORDER, G_TRAVERSE_ALL, -1, post_order_analysis, NULL);

    // printf("\npre_order_analysis\n");
    g_node_traverse (root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, pre_order_analysis, NULL);

    bool cond = (main_count > 0);
    cond ? : error("main function missing", 1);
}