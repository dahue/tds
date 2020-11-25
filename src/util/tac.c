#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "symbol_table.h"

int reg_count;
int offset;
GList *tac_list;

// GList *statement_list;
GList *tac_func;

GList *if_list;

void load_const(struct Symbol *s){
    printf("Load_Const %s, R_%d\n", s->name, s->reg);
}

void load_mem(struct Symbol *s){
    printf("Load_Mem %s, R_%d\n", s->name, s->reg);
}

void store_mem(struct Symbol *n, struct Symbol *m){
    printf("Store_Mem R_%d, %s\n", m->reg, n->name);
}

void add_reg(struct Symbol *n, struct Symbol *m, struct Symbol *r){
    printf("Add_Reg R_%d, R_%d, R_%d\n", n->reg, m->reg, r->reg);
}

void mul_reg(struct Symbol *n, struct Symbol *m, struct Symbol *r){
    printf("Mul_Reg R_%d, R_%d, R_%d\n", n->reg, m->reg, r->reg);
}

void and_reg(struct Symbol *n, struct Symbol *m, struct Symbol *r){
    printf("AND_Reg R_%d, R_%d, R_%d\n", n->reg, m->reg, r->reg);
}

void eq_reg(struct Symbol *n, struct Symbol *m, struct Symbol *r){
    printf("EQ_Reg R_%d, R_%d, R_%d\n", n->reg, m->reg, r->reg);
}

void not_reg(struct Symbol *n, struct Symbol *r){
    printf("NOT_Reg R_%d, R_%d\n", n->reg, r->reg);
}

void less_reg(struct Symbol *n, struct Symbol *m, struct Symbol *r){
    printf("LESS_Reg R_%d, R_%d, R_%d\n", n->reg, m->reg, r->reg);
}

void return_reg(struct Symbol *r){
    if (r->type == T_VOID){
        printf("Return_Reg %s\n", "VOID");
    }
    else{
        printf("Return_Reg R_%d\n", r->reg);
    }
}

void start_func(struct Symbol *s){
    printf("START_FUNC %s\n", s->name);
}

void end_func(struct Symbol *s){
    printf("END_FUNC %s\n", s->name);
}

void call_func(struct Symbol *s){
    if (g_list_length(s->param) == 0){
        printf("CALL_FUNC %s, R_%d\n", s->name, s->reg);
    }
    else if (g_list_length(s->param) == 1){
        struct Symbol *x = (struct Symbol *)g_list_nth_data(s->param, 0);
        printf("CALL_FUNC %s, %s, R_%d\n", s->name, x->name, s->reg);
    }
    else if (g_list_length(s->param) == 2){
        struct Symbol *x = (struct Symbol *)g_list_nth_data(s->param, 0);
        struct Symbol *y = (struct Symbol *)g_list_nth_data(s->param, 1);
        printf("CALL_FUNC %s, %s, %s, R_%d\n", s->name, x->name, y->name, s->reg);
    }
}
// void if_stmt(struct Symbol *c, struct Symbol *t, struct Symbol *s){
//     printf("IF_cond R_%d, R_%d\n", c->reg, s->reg);
// }

gboolean tac_generator(GNode *node, gpointer data){
    struct Symbol *s = node->data;
    if (s->reg == -1) {
        switch(s->flag){
            case F_ASSIGN_OP:
                tac_func = g_list_append(tac_func, node);
                break;
            case F_FUNC:
                tac_func = g_list_prepend(tac_func, node);
                struct Symbol *a = newSymbol();
                a->name = s->name;
                a->flag = F_END_FUNC;
                s->offset = offset;
                tac_func = g_list_append(tac_func, g_node_new(a));
                tac_list = g_list_concat(tac_list, tac_func);
                tac_func = NULL;
                offset = 8;
                break;
            case F_CONST:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_ID:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_ID_PARAM:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_ID_GLOBAL:
                s->reg = reg_count;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                break;
            case F_PLUS_OP:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_MUL_OP:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_AND_OP:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_EQ_OP:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_NOT_OP:
                s->reg = reg_count;
                s->offset = offset;
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;
                break;
            case F_RETURN:
                if (s->type != T_VOID){
                    struct Symbol *l = g_node_nth_child(node, 0)->data;
                    s->reg = l->reg;
                }
                tac_func = g_list_append(tac_func, node);
                break;
            case F_LESS_OP:
                s->reg = reg_count;
                s->offset = offset;
                struct Symbol *parent_symbol = (node->parent)->data;
                if ((parent_symbol->flag == F_IF) && (parent_symbol->flag == F_IF_ELSE)) {
                    if_list = g_list_append(if_list, node);
                }
                else{
                    tac_func = g_list_append(tac_func, node);
                }
                reg_count += 1;
                offset += 8;
                break;
            case F_FUNC_CALL:
                s->reg = reg_count;
                s->offset = offset;// + 8 * g_list_length(s->param);
                tac_func = g_list_append(tac_func, node);
                reg_count += 1;
                offset += 8;// + 8 * g_list_length(s->param);
                break;
            // case F_IF:
            //     s->reg = reg_count;
            //     s->offset = offset;
            //     tac_func = g_list_append(tac_func, node);
            //     reg_count += 1;
            //     offset += 8;
            //     break;
        }
    }
    return false;
}

void show_tac_list(GList *list) {
    int i;
    for (i=0; i < g_list_length(list); i++){
        GNode *node = g_list_nth_data(list, i);
        struct Symbol *s = node->data;
        // printf("%s, R_%d\n", s->name, s->reg);
        if (s->flag == F_FUNC) {
            start_func(s);
        }
        if (s->flag == F_END_FUNC) {
            end_func(s);
        }
        if (s->flag == F_CONST) {
            load_const(s);
        }
        if ((s->flag == F_ID) || (s->flag == F_ID_GLOBAL) || (s->flag == F_ID_PARAM)) {
            load_mem(s);
        }
        if (s->flag == F_RETURN) {
            return_reg(s);
        }
        if (s->flag == F_NOT_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            not_reg(l, s);
        }
        if (s->flag == F_ASSIGN_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            store_mem(l, r);
        }
        if (s->flag == F_PLUS_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            add_reg(l, r, s);
        }
        if (s->flag == F_MUL_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            mul_reg(l, r, s);
        }
        if (s->flag == F_AND_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            and_reg(l, r, s);
        }
        if (s->flag == F_EQ_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            eq_reg(l, r, s);
        }
        if (s->flag == F_LESS_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            less_reg(l, r, s);
        }
        if (s->flag == F_FUNC_CALL) {
            call_func(s);
        }
        // if (s->flag == F_IF) {
        //     struct Symbol *c = g_node_nth_child(node, 0)->data;
        //     struct Symbol *t = g_node_nth_child(node, 1)->data;
        //     if_stmt(c, t, s);
        // }
    }
}

GList* tac(GNode* root){
    reg_count = 0;
    offset = 8;
    g_node_traverse (root, G_POST_ORDER, G_TRAVERSE_ALL, -1, tac_generator, NULL);
    // show_tac_list(tac_list);
    return tac_list;
}