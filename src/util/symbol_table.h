#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <glib.h>

struct Symbol {
    char* flag;
    char* name;
    char* type;
    int value;
    struct Symbol* params;
};

// struct Node { 
//     struct Symbol* data; 
//     struct Node* next;
// };

// struct StackNode {
//     struct Node* top;
//     struct StackNode* next;
// };

GList *newSymbolTable();

GList *addLevel(GList *stack);

GList *removeLevel(GList *stack);

GList *insertSymbol(GList *stack, struct Symbol *symbol);

struct Symbol *findSymbol(GList *stack, struct Symbol *symbol);

#endif
