#include <glib.h>
#include "symbol_table.h"

GList asm_code_list;

char *gen_header(GList *list){
    printf("\t.data\n");
    int i;
    for (i=0; i < g_list_length(list); i++){
        GNode *node = g_list_nth_data(list, i);
        struct Symbol *s = node->data;
        if (s->flag == F_ID_GLOBAL) {
            printf("%s:\t.long\t0\n", s->name);
        }
    }
}

char* gen_start_func(struct Symbol *s){
    printf("\t.text\n\t.global\t%s\n\t.type\t%s, @function\n", s->name, s->name);
    printf("%s:\n", s->name);
    printf("\tpushq\t%%rbp\n\tmovq\t%%rsp, %%rbp\n\tsubq\t$%d, %%rsp\n", s->offset);
}

char* gen_end_func(struct Symbol *s){
    printf("\tleave\n\tret\n");
}

char* gen_load_const(struct Symbol *s){
    if (s->type == T_INTEGER){
        printf("\tmovq\t$%d, -%d(%%rbp)\n", s->value_int, s->offset);
    }
    else{
        printf("\tmovq\t$%d, -%d(%%rbp)\n", s->value_bool, s->offset);
    }
}

char* gen_load_mem(struct Symbol *s){
    // printf("Load_Mem %s, R_%d\n", s->name, s->reg);
    if (s->flag == F_ID_GLOBAL) {
        if (s->type == T_INTEGER){
            printf("\tmovq\t$%d, %s(%%rip)\n", s->value_int, s->name);
        }
        else{
            printf("\tmovq\t$%d, %s(%%rip)\n", s->value_bool, s->name);
        }
        
    }
    else if (s->flag == F_ID){
        if (s->type == T_INTEGER){
            printf("\tmovq\t$%d, -%d(%%rbp)\n", s->value_int, s->offset);
        }
        else{
            printf("\tmovq\t$%d, -%d(%%rbp)\n", s->value_bool, s->offset);
        }
    }
}

char* gen_store_mem(struct Symbol *l, struct Symbol *r){
    if (l->flag == F_ID_GLOBAL) {
        if (r->flag == F_ID_GLOBAL){
            printf("\tmovq\t%s(%%rip), %%rbx\n", r->name);
            printf("\tmovq\t%%rbx, %s(%%rip)\n", l->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rbx\n", r->offset);
            printf("\tmovq\t%%rbx, %s(%%rip)\n", l->name);
        }
    }
    else if (l->flag == F_ID){
        if (r->flag == F_ID_GLOBAL){
            printf("\tmovq\t%s(%%rip), %%rbx\n", r->name);
            printf("\tmovq\t%%rbx, -%d(%%rbp)\n", l->offset);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rbx\n", r->offset);
            printf("\tmovq\t%%rbx, -%d(%%rbp)\n", l->offset);
        }
    }
}

char* gen_and_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\tandq\t%s(%%rip), %%rax\n", l->name);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
    else{
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else {
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\tandq\t-%d(%%rbp), %%rax\n", l->offset);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
}

char* gen_add_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\taddq\t%s(%%rip), %%rax\n", l->name);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
    else{
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\taddq\t-%d(%%rbp), %%rax\n", l->offset);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
}

char* gen_mul_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\timulq\t%s(%%rip), %%rax\n", l->name);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
    else{
        if (r->offset == 0){
            printf("\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        printf("\timulq\t-%d(%%rbp), %%rax\n", l->offset);
        printf("\tmovq\t%%rax, -%d(%%rbp)\n", s->offset);
    }
}

char* gen_return_reg(struct Symbol *s, struct Symbol *l){
    if (s->type == T_VOID){
        printf("skip");
    }
    else{
        if (l->flag == F_ID_GLOBAL) {
            printf("\tmovq\t%s(%%rip), %%rax\n", l->name);
        }
        else{
            printf("\tmovq\t-%d(%%rbp), %%rax\n", l->offset);
        }
    }
}

void generate_assembler_code(GList *list) {
    gen_header(list);
    int i;
    for (i=0; i < g_list_length(list); i++){
        GNode *node = g_list_nth_data(list, i);
        struct Symbol *s = node->data;
        if (s->flag == F_FUNC) {
            gen_start_func(s);
        }
        if (s->flag == F_END_FUNC) {
            gen_end_func(s);
        }
        if (s->flag == F_CONST) {
            gen_load_const(s);
        }
        if ((s->flag == F_ID) || (s->flag == F_ID_GLOBAL)) {
            gen_load_mem(s);
        }
        if (s->flag == F_RETURN) {
            struct Symbol *l;
            if (s->type != T_VOID){
                l = g_node_nth_child(node, 0)->data;
            }
            gen_return_reg(s, l);
        }
        if (s->flag == F_NOT_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            not_reg(l, s);
        }
        if (s->flag == F_ASSIGN_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_store_mem(l, r);
        }
        if (s->flag == F_PLUS_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_add_reg(l, r, s);
        }
        if (s->flag == F_MUL_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_mul_reg(l, r, s);
        }
        if (s->flag == F_AND_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_and_reg(l, r, s);
        }
        if (s->flag == F_EQ_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            eq_reg(l, r, s);
        }
    }
}
