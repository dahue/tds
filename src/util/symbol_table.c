#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmodule.h>
#include "symbol_table.h"
  
struct Symbol *newSymbol(enum flag flag, char* name, enum type type){
   struct Symbol* symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
   symbol->flag = flag;
   symbol->name = name;
   symbol->type = type;
   symbol->value = 0;
   symbol->param = NULL;
   return symbol;
}

GList *newSymbolTable(){
   GList *stack = NULL;
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

struct Symbol *findSymbolRecursively(GList *stack_level, struct Symbol *symbol){
   if (stack_level == NULL){
      return NULL;
   }
   GHashTable *current_level = stack_level->data;
   struct Symbol *res = g_hash_table_lookup(current_level, symbol->name);
   if (res != NULL) {
      return res;
   }
   else{
      return findSymbolRecursively(g_list_next(stack_level), symbol);
   }
}

struct Symbol *findSymbol(GList *stack, struct Symbol *symbol){
   return findSymbolRecursively(stack, symbol);
}

// int main(void)
// {
//    GList *symbol_table = newSymbolTable();
   
//    struct Symbol *s = newSymbol(E_ID, "x", E_INTEGER);
//    s->value = 6;
   
//    symbol_table = insertSymbol(symbol_table, s);

//    symbol_table = addLevel(symbol_table);

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_hash_table_size(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_hash_table_size(g_list_last(symbol_table)->data));

//    struct Symbol *s2 = newSymbol(E_ID, "y", E_INTEGER);
//    s2->value = 4;
//    symbol_table = insertSymbol(symbol_table, s2);

//    struct Symbol *s3 = newSymbol(E_ID, "x", E_INTEGER);
//    s3->value = 5;
//    symbol_table = insertSymbol(symbol_table, s3);

//    // symbol_table = removeLevel(symbol_table);

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_hash_table_size(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_hash_table_size(g_list_last(symbol_table)->data));

//    struct Symbol * r = findSymbol(symbol_table, s);
//    if (r != NULL){
//       printf("'%s' found -> '%d'\n", r->name, r->value);
//    }
//    else{
//       printf("Not found\n");
//    }
// }


