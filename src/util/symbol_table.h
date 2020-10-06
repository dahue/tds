#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <glib.h>

enum flag{E_ID, E_FUNC, E_PARAM};
enum type{E_VOID, E_INTEGER, E_BOOL};

struct Symbol {
    enum flag flag;
    char* name;
    enum type type;
    int value;
    struct Symbol* param;
};

GList *newSymbolTable();

GList *addLevel(GList *stack);

GList *removeLevel(GList *stack);

GList *insertSymbol(GList *stack, struct Symbol *symbol);

struct Symbol *findSymbol(GList *stack, struct Symbol *symbol);

#endif
