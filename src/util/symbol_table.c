#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
   GList *list = NULL;
   stack = g_list_append(stack, list);
   return stack;
}

GList *addLevel(GList *stack){
   GList *newLevel = NULL;
   stack = g_list_prepend(stack, newLevel);
   return stack;
}

GList *removeLevel(GList *stack){
   stack = g_list_remove_link(stack, stack);
   return stack;
}

GList *insertSymbol(GList *stack, struct Symbol *symbol){
   stack->data = g_list_prepend(stack->data, symbol);
   return stack;
}

gint comparator(gconstpointer a, gconstpointer b){
   struct Symbol *symbol1 = (struct Symbol *)a;
   struct Symbol *symbol2 = (struct Symbol *)b;
   char* str1 = symbol1->name;
   char* str2 = symbol2->name;
   return strcmp (str1, str2);
}

struct Symbol *findSymbolRecursively(GList *stack_level, struct Symbol *symbol){
   if (stack_level == NULL){
      return NULL;
   }
   GList *current_level = stack_level->data;
   GList *res = g_list_find_custom(current_level, symbol, &comparator);
   if (res != NULL) {
      return res->data;
   }
   else{
      return findSymbolRecursively(g_list_previous(stack_level), symbol);
   }
}

struct Symbol *findSymbol(GList *stack, struct Symbol *symbol){
   return findSymbolRecursively(g_list_last(stack), symbol);
}

// int main(void)
// {
//    GList *symbol_table = newSymbolTable();
   
//    struct Symbol *s = newSymbol(E_ID, "x", E_INTEGER);
//    s->value = 6;
   
//    symbol_table = insertSymbol(symbol_table, s);

//    symbol_table = addLevel(symbol_table);

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_list_length(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_list_length(g_list_last(symbol_table)->data));

//    struct Symbol *s2 = newSymbol(E_ID, "y", E_INTEGER);
//    s2->value = 4;
//    symbol_table = insertSymbol(symbol_table, s2);

//    struct Symbol *s3 = newSymbol(E_ID, "y", E_INTEGER);
//    s3->value = 5;
//    symbol_table = insertSymbol(symbol_table, s3);

//    symbol_table = removeLevel(symbol_table);

//    printf("Symbol table is %d items long\n", g_list_length(symbol_table));
//    printf("Top of symbol list is %d items long\n", g_list_length(symbol_table->data));
//    printf("Bottom of symbol list is %d items long\n", g_list_length(g_list_last(symbol_table)->data));

//    struct Symbol * r = findSymbol(symbol_table, s);
//    if (r != NULL){
//       printf("'%s' found -> '%d'\n", r->name, r->value);
//    }
//    else{
//       printf("Not found\n");
//    }
// }


