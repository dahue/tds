#include <glib.h>
#include <stdio.h>
#include "symbol_table.h"

char *gen_header(GList *list, FILE *fp){
    fprintf(fp, "\t.data\n");
    int i;
    for (i=0; i < g_list_length(list); i++){
        GNode *node = g_list_nth_data(list, i);
        struct Symbol *s = node->data;
        if (s->flag == F_ID_GLOBAL) {
            fprintf(fp, "%s:\t.long\t0\n", s->name);
        }
    }
}

char* gen_start_func(struct Symbol *s, FILE *fp){
    fprintf(fp, 
        "\t.text\n\t.global\t%s\n\t.type\t%s, @function\n" \
        "%s:\n" \
        "\tpushq\t%%rbp\n\tmovq\t%%rsp, %%rbp\n\tsubq\t$%d, %%rsp\n", 
        s->name, s->name, s->name, s->offset
    );
}

char* gen_end_func(struct Symbol *s, FILE *fp){
    fprintf(fp, "\tleave\n\tret\n");
}

char* gen_load_const(struct Symbol *s, FILE *fp){
    if (s->type == T_INTEGER){
        fprintf(fp, "\tmovq\t$%d, -%d(%%rbp)\n", s->value_int, s->offset);
    }
    else{
        fprintf(fp, "\tmovq\t$%d, -%d(%%rbp)\n", s->value_bool, s->offset);
    }
}

char* gen_load_mem(struct Symbol *s, FILE *fp){
    if (s->flag == F_ID_GLOBAL) {
        if (s->type == T_INTEGER){
            fprintf(fp, "\tmovq\t$%d, %s(%%rip)\n", s->value_int, s->name);
        }
        else{
            fprintf(fp, "\tmovq\t$%d, %s(%%rip)\n", s->value_bool, s->name);
        }
        
    }
    else if (s->flag == F_ID){
        if (s->type == T_INTEGER){
            fprintf(fp, "\tmovq\t$%d, -%d(%%rbp)\n", s->value_int, s->offset);
        }
        else{
            fprintf(fp, "\tmovq\t$%d, -%d(%%rbp)\n", s->value_bool, s->offset);
        }
    }
}

char* gen_store_mem(struct Symbol *l, struct Symbol *r, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        if (r->flag == F_ID_GLOBAL){
            fprintf(fp, 
                "\tmovq\t%s(%%rip), %%rbx\n" \
                "\tmovq\t%%rbx, %s(%%rip)\n", 
                r->name, l->name
            );
        }
        else{
            fprintf(fp, 
                "\tmovq\t-%d(%%rbp), %%rbx\n" \
                "\tmovq\t%%rbx, %s(%%rip)\n", 
                r->offset, l->name);
        }
    }
    else if (l->flag == F_ID){
        if (r->flag == F_ID_GLOBAL){
            fprintf(fp, 
                "\tmovq\t%s(%%rip), %%rbx\n" \
                "\tmovq\t%%rbx, -%d(%%rbp)\n", 
                r->name, l->offset);
        }
        else{
            fprintf(fp, 
                "\tmovq\t-%d(%%rbp), %%rbx\n" \
                "\tmovq\t%%rbx, -%d(%%rbp)\n", 
                r->offset, l->offset);
        }
    }
}

char* gen_and_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\tandq\t%s(%%rip), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->name, s->offset
        );
    }
    else{
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else {
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\tandq\t-%d(%%rbp), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->offset, s->offset
        );
    }
}

char* gen_add_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\taddq\t%s(%%rip), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->name, s->offset
        );
    }
    else{
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\taddq\t-%d(%%rbp), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->offset, s->offset
        );
    }
}

char* gen_mul_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\timulq\t%s(%%rip), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->name, s->offset
        );
    }
    else{
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\timulq\t-%d(%%rbp), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->offset, s->offset
        );
    }
}

char* gen_return_reg(struct Symbol *s, struct Symbol *l, FILE *fp){
    if (s->type != T_VOID){
        if (l->flag == F_ID_GLOBAL) {
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", l->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", l->offset);
        }
    }
}

char* gen_not_reg(struct Symbol *l, struct Symbol *s, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        fprintf(fp, 
            "\tmovq\t$1, %%rax\n" \
            "\txorq\t%s(%%rip), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->name, s->offset
        );
    }
    else{
        fprintf(fp,
            "\tmovq\t$1, %%rax\n" \
            "\txorq\t-%d(%%rbp), %%rax\n" \
            "\tmovq\t%%rax, -%d(%%rbp)\n", 
            l->offset, s->offset
        );
    }
}

char* gen_eq_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s, FILE *fp){
    if (l->flag == F_ID_GLOBAL) {
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, "\tcmpq\t%s(%%rip), %%rax\n", l->name);
    }
    else{
        if (r->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", r->name);
        }
        else {
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", r->offset);
        }
        fprintf(fp, 
            "\tcmpq\t-%d(%%rbp), %%rax\n", l->offset);
    }
    fprintf(fp,"\tje\tequal_%d\n\tjmp\tnotequal_%d\n", s->offset, s->offset);
    fprintf(fp,"equal_%d:\n\tmovq\t$1, -%d(%%rbp)\n\tjmp\tcontinue_%d\n", s->offset, s->offset, s->offset);
    fprintf(fp,"notequal_%d:\n\tmovq\t$0, -%d(%%rbp)\ncontinue_%d:\n", s->offset, s->offset, s->offset);
}

char* gen_less_reg(struct Symbol *l, struct Symbol *r, struct Symbol *s, FILE *fp){
    if (r->flag == F_ID_GLOBAL) {
        if (l->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", l->name);
        }
        else{
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", l->offset);
        }
        fprintf(fp, "\tcmpq\t%s(%%rip), %%rax\n", r->name);
    }
    else{
        if (l->offset == 0){
            fprintf(fp, "\tmovq\t%s(%%rip), %%rax\n", l->name);
        }
        else {
            fprintf(fp, "\tmovq\t-%d(%%rbp), %%rax\n", l->offset);
        }
        fprintf(fp, 
            "\tcmpq\t-%d(%%rbp), %%rax\n", r->offset);
    }
    fprintf(fp,"\tjl\tless_%d\n\tjmp\tnotless_%d\n", s->offset, s->offset);
    fprintf(fp,"less_%d:\n\tmovq\t$1, -%d(%%rbp)\n\tjmp\tcontinue_%d\n", s->offset, s->offset, s->offset);
    fprintf(fp,"notless_%d:\n\tmovq\t$0, -%d(%%rbp)\ncontinue_%d:\n", s->offset, s->offset, s->offset);
}

void generate_assembler_code(GList *list, FILE *fp) {
    gen_header(list, fp);
    int i;
    for (i=0; i < g_list_length(list); i++){
        GNode *node = g_list_nth_data(list, i);
        struct Symbol *s = node->data;
        if (s->flag == F_FUNC) {
            gen_start_func(s, fp);
        }
        if (s->flag == F_END_FUNC) {
            gen_end_func(s, fp);
        }
        if (s->flag == F_CONST) {
            gen_load_const(s, fp);
        }
        if ((s->flag == F_ID) || (s->flag == F_ID_GLOBAL)) {
            gen_load_mem(s, fp);
        }
        if (s->flag == F_RETURN) {
            struct Symbol *l;
            if (s->type != T_VOID){
                l = g_node_nth_child(node, 0)->data;
            }
            gen_return_reg(s, l, fp);
        }
        if (s->flag == F_NOT_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            gen_not_reg(l, s, fp);
        }
        if (s->flag == F_ASSIGN_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_store_mem(l, r, fp);
        }
        if (s->flag == F_PLUS_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_add_reg(l, r, s, fp);
        }
        if (s->flag == F_MUL_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_mul_reg(l, r, s, fp);
        }
        if (s->flag == F_AND_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_and_reg(l, r, s, fp);
        }
        if (s->flag == F_EQ_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_eq_reg(l, r, s, fp);
        }
        if (s->flag == F_LESS_OP) {
            struct Symbol *l = g_node_nth_child(node, 0)->data;
            struct Symbol *r = g_node_nth_child(node, 1)->data;
            gen_less_reg(l, r, s, fp);
        }
    }
}
