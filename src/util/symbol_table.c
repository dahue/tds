#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "symbol_table.h"
  
GList *newSymbolTable(){
   GList *stack = NULL;
   GList *list = NULL;
   stack = g_list_append(stack, list);
   return stack;
}

GList *addLevel(GList *stack){
   GList *newLevel = NULL;
   stack = g_list_append(stack, newLevel);
   return stack;
}

GList *removeLevel(GList *stack){
   stack = g_list_remove_link(stack, g_list_last(stack));
   return stack;
}

GList *insertSymbol(GList *stack, struct Symbol *symbol){
   g_list_last(stack)->data = g_list_append(g_list_last(stack)->data, symbol);
   return stack;
}

struct Symbol *findSymbol(GList *stack, struct Symbol *symbol);

int main(void)
{
   // GList *stack = NULL;
   // GList *list = NULL;
   // list = g_list_append (list, "first");
   // list = g_list_append (list, "second");
   // stack = g_list_append(stack, list);
   // printf("The list is now %d items long\n", g_list_length(list));
   // printf("The first item is '%s'\n", (char*)g_list_first(list)->data);
   // printf("The first item is '%s'\n", (char*)g_list_first(list)->data);

   GList *symbol_table = newSymbolTable();
   symbol_table = addLevel(symbol_table);
   struct Symbol *s = (struct Symbol*)malloc(sizeof(struct Symbol));
   s->value = 6;
   s->name = "x";
   symbol_table = insertSymbol(symbol_table, s);
   // printf("The first item is '%s'\n", (char*)(g_list_first(symbol_table)->data)->data);
   GList * a = g_list_last(symbol_table)->data;
   printf("The list is now %d items long\n", g_list_length(symbol_table));
   printf("The list is now %d items long\n", g_list_length(g_list_last(symbol_table)->data));
   printf("The list is now %d items long\n", g_list_length(symbol_table->data));
   struct Symbol * b = a->data;

   int c = b->value;

   printf("The first item is '%d'\n", c);

}


// gcc ‑I/usr/include/glib‑2.0 ‑I/usr/lib/glib‑2.0/include ‑lglib‑2.0 ‑o ex‑compile ex‑compile.c