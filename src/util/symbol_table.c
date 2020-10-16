#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmodule.h>
#include "symbol_table.h"

// GList *stack = NULL;

struct Symbol *newSymbol(){
   struct Symbol* symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
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


// int main(void)
// {
//    GList *symbol_table = newSymbolTable();
   
//    struct Symbol *s = newSymbol(E_ID, "x", E_INTEGER);
//    s->value = 6;
   
//    symbol_table = insertSymbol(s);

//    symbol_table = addLevel();

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_hash_table_size(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_hash_table_size(g_list_last(symbol_table)->data));

//    struct Symbol *s2 = newSymbol(E_ID, "y", E_INTEGER);
//    s2->value = 4;
//    symbol_table = insertSymbol(s2);

//    struct Symbol *s3 = newSymbol(E_ID, "x", E_INTEGER);
//    s3->value = 5;
//    symbol_table = insertSymbol(s3);

//    symbol_table = removeLevel();

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_hash_table_size(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_hash_table_size(g_list_last(symbol_table)->data));

//    struct Symbol * r = findSymbol(s2);
//    if (r != NULL){
//       printf("'%s' found -> '%d'\n", r->name, r->value);
//    }
//    else{
//       printf("Not found\n");
//    }
// }