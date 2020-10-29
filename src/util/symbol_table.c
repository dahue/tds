#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmodule.h>
#include "symbol_table.h"

struct Symbol *newSymbol(){
   struct Symbol* symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
   symbol->name = "e_indf";
   symbol->flag = F_INDF;
   symbol->type = T_INDF;
   symbol->reg = -1;
   symbol->lineno = 0;
   return symbol;
}

char *toString(struct Symbol *s){
   char *str;
   // asprintf(&str, "flag: %s, type: %s, name: %s, value: %d, param %s", str, s->flag, s->type, s->name, s->value, s->param);
   return str;
}

GList *newSymbolTable(GList *stack){
   GHashTable *hash_table = g_hash_table_new(g_str_hash, g_str_equal);
   stack = g_list_prepend(stack, hash_table);
   return stack;
}

GList *addLevel(GList *stack){
   GHashTable *newLevel = g_hash_table_new(g_str_hash, g_str_equal);
   stack = g_list_prepend(stack, newLevel);
   return stack;
}

GList *removeLevel(GList *stack){
   stack = g_list_remove_link(stack, stack);
   return stack;
}

GList *insertSymbol(GList *stack, struct Symbol *symbol){
   g_hash_table_insert (stack->data, symbol->name, symbol);
   return stack;
}

struct Symbol *exists(GList *stack, char *name){
   GHashTable *current_level = stack->data;
   struct Symbol *res = g_hash_table_lookup(current_level, name);
   return res;
}

struct Symbol *findSymbolRecursively(GList *stack_level, char *name){
   if (stack_level == NULL){
      return NULL;
   }
   GHashTable *current_level = stack_level->data;
   struct Symbol *res = g_hash_table_lookup(current_level, name);
   if (res != NULL) {
      return res;
   }
   else{
      return findSymbolRecursively(g_list_next(stack_level), name);
   }
}

struct Symbol *findSymbol(GList *stack, char *name){
   return findSymbolRecursively(stack, name);
}
